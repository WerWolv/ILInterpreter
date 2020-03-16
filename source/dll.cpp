#include "dll.hpp"

#include "types.hpp"
#include "file_headers.hpp"
#include "tables.hpp"
#include "logger.hpp"

#include <string>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <codecvt>
#include <locale>

namespace ili {

    DLL::DLL(std::string filePath) {
        FILE *dllFile = fopen(filePath.c_str(), "rb");

        if (dllFile == nullptr) {
            Logger::error("Cannot open file %s!", filePath.c_str());
            exit(1);
        }

        fseek(dllFile, 0, SEEK_END);

        this->m_fileSize = ftell(dllFile);
        this->m_dllData = new u8[this->m_fileSize];
        rewind(dllFile);
        fread(this->m_dllData, 1, this->m_fileSize, dllFile);
        fclose(dllFile);

        this->m_dosHeader = reinterpret_cast<dos_header_t*>(this->m_dllData);
        this->m_dosStub = reinterpret_cast<dos_stub_t*>(OFFSET(this->m_dosHeader, sizeof(dos_header_t)));
        this->m_ntHeader = reinterpret_cast<nt_header_t*>(OFFSET(this->m_dosStub, sizeof(dos_stub_t)));
        this->m_optionalHeader = reinterpret_cast<optional_header_t*>(OFFSET(this->m_ntHeader, sizeof(nt_header_t)));


        for (u8 section = 0; section < this->m_ntHeader->numSections; section++)
            this->m_sectionTable.push_back(reinterpret_cast<section_table_entry_t*>(OFFSET(this->m_optionalHeader, sizeof(optional_header_t) + section * sizeof(section_table_entry_t))));

        section_table_entry_t *crlSection = this->getVirtualSection(this->m_optionalHeader->crlRuntimeHeader.rva);
        this->m_crlRuntimeHeader = reinterpret_cast<crl_runtime_header_t*>(OFFSET(this->m_dllData, VRA_TO_OFFSET(crlSection, this->m_optionalHeader->crlRuntimeHeader.rva)));

        section_table_entry_t *metadataSection = this->getVirtualSection(this->m_crlRuntimeHeader->metaData.rva);
        u8 *metadataBase = OFFSET(this->m_dllData, VRA_TO_OFFSET(metadataSection, this->m_crlRuntimeHeader->metaData.rva));
        u8 *currentDataPtr = metadataBase;

        // Parse Metadata
        {

            std::memcpy(&this->m_metadata, currentDataPtr, offsetof(metadata_t, version));
            currentDataPtr += offsetof(metadata_t, version);
            std::memcpy(&this->m_metadata.version, currentDataPtr, this->m_metadata.length);
            currentDataPtr += this->m_metadata.length;
            std::memcpy(&this->m_metadata.flags, currentDataPtr, 2 * sizeof(u16));
            currentDataPtr += 2 * sizeof(u16);

        }

        // Parse Stream Headers
        {
            stream_header_t *currHeader = reinterpret_cast<stream_header_t*>(currentDataPtr);
            for (u8 stream = 0; stream < this->m_metadata.streams; stream++) {
                this->m_streamHeaders.push_back(currHeader);
                currentDataPtr += (2 * sizeof(u32)) + ALIGN(strlen(currHeader->name), 4);
                currHeader = reinterpret_cast<stream_header_t*>(currentDataPtr);
            }

        }

        // Parse #~ Stream rows and tables
        {
            for (u8 stream = 0; stream < this->m_metadata.streams; stream++) {
                if (std::string(this->m_streamHeaders[stream]->name) == "#~") {
                    tilde_stream_t *tildeStream = reinterpret_cast<tilde_stream_t*>(OFFSET(metadataBase, this->m_streamHeaders[stream]->offset));

                    currentDataPtr += 24; // Skip to rows array

                    for (u8 i = 0; i < 64; i++) {
                        if ((tildeStream->valid & (1ULL << i)) == (1ULL << i)) {
                            this->m_numRows[i] = *reinterpret_cast<u32*>(currentDataPtr);
                            currentDataPtr += sizeof(u32);
                        }
                    }

                    for (u8 i = 0; i < 64; i++) {
                        u32 count = this->m_numRows[i];

                        this->m_tildeTableData.push_back({});
                        if (count != 0) {
                            u8 tableSize = getMetadataTableSize(i);
                            for (u16 j = 0; j < count; j++) {
                                this->m_tildeTableData[i].push_back({ currentDataPtr, tableSize });
                                currentDataPtr += tableSize;
                            }
                        }
                    }
                } else if (std::string(this->m_streamHeaders[stream]->name) == "#Strings") {
                    this->m_stringsHeap = OFFSET(metadataBase, this->m_streamHeaders[stream]->offset);
                } else if (std::string(this->m_streamHeaders[stream]->name) == "#US") {
                    this->m_userStringsHeap = OFFSET(metadataBase, this->m_streamHeaders[stream]->offset);
                } else if (std::string(this->m_streamHeaders[stream]->name) == "#Blob") {
                    this->m_blobHeap = OFFSET(metadataBase, this->m_streamHeaders[stream]->offset);
                }
            }
        }
    }

    DLL::~DLL() {
        delete[] this->m_dllData;
    }

    void DLL::validate() {
        if (this->m_dosHeader->magic != 0x5A4D) {
            Logger::error("Invalid DOS Header!");
            exit(1);
        } else Logger::info("Valid DOS Header!");

        if (this->m_ntHeader->magic != 0x00004550) {
            Logger::error("Invalid NT Header!");
            exit(1);
        } else Logger::info("Valid NT Header!");

        Logger::info("Stack size: %lx", this->getStackSize());

        if (this->m_crlRuntimeHeader->headerSize != sizeof(crl_runtime_header_t)) {
            Logger::error("Invalid CLR Header!");
            exit(1);
        } else Logger::info("Valid CLR Header!");

        Logger::info("Runtime version: %d.%d", this->m_crlRuntimeHeader->runtimeVersionMajor, this->m_crlRuntimeHeader->runtimeVersionMinor);
        Logger::info("Entrypoint Token: %x", this->m_crlRuntimeHeader->entryPointToken);

        if (this->m_metadata.magic != 0x424A5342) {
            Logger::error("Invalid Metadata Header!");
            exit(1);
        } else Logger::info("Valid Metadata Header!");

        Logger::info(".NET Framework version: %s", this->m_metadata.version);

        for (u8 stream = 0; stream < this->m_metadata.streams; stream++) {
            Logger::info("Found Stream: %s", this->m_streamHeaders[stream]->name);

            if (std::string(this->m_streamHeaders[stream]->name) == "#~") {
                for (u8 i = 0; i < 64; i++)
                    if (this->m_numRows[i] != 0)
                        Logger::info("  Table 0x%X: %u entries", i, this->m_numRows[i]);
            }
        }
    }

    table_method_def_t* DLL::getMethodDefByMetadataToken(u32 token) {
        if (TABLE_ID(token) == TABLE_ID_METHODDEF)
            return reinterpret_cast<table_method_def_t*>(this->m_tildeTableData[TABLE_ID(token)][TABLE_INDEX(token) - 1].base);
        else return nullptr;
    }

    table_member_ref_t* DLL::getMemberRefByMetadataToken(u32 token) {
        if (TABLE_ID(token) == TABLE_ID_MEMBERREF)
            return reinterpret_cast<table_member_ref_t*>(this->m_tildeTableData[TABLE_ID(token)][TABLE_INDEX(token) - 1].base);
        else return nullptr;
    }

    table_method_def_t * DLL::getMethodDefByIndex(u32 index) {
        return reinterpret_cast<table_method_def_t*>(this->m_tildeTableData[TABLE_ID_METHODDEF][index - 1].base);
    }

    table_type_ref_t* DLL::getTypeRefByIndex(u32 index) {
        return reinterpret_cast<table_type_ref_t*>(this->m_tildeTableData[TABLE_ID_TYPEREF][index - 1].base);
    }

    table_type_def_t* DLL::getTypeDefByIndex(u32 index) {
        return reinterpret_cast<table_type_def_t*>(this->m_tildeTableData[TABLE_ID_TYPEDEF][index - 1].base);
    }

    table_assembly_ref_t* DLL::getAssemblyRefByIndex(u32 index) {
        return reinterpret_cast<table_assembly_ref_t*>(this->m_tildeTableData[TABLE_ID_ASSEMBLYREF][index - 1].base);
    }

    table_field_t* DLL::getFieldByIndex(u32 index) {
        return reinterpret_cast<table_field_t*>(this->m_tildeTableData[TABLE_ID_FIELD][index - 1].base);
    }

    u32 DLL::getEntryMethodToken() {
        return this->m_crlRuntimeHeader->entryPointToken;
    }

    const char* DLL::getString(u32 index) {
        return reinterpret_cast<char*>(&this->m_stringsHeap[index]);
    }

    u32 DLL::getBlobSize(u32 index) {
        switch (getBlobHeaderSize(index)) {
            case 1: return this->m_blobHeap[index];
            case 2: return ((this->m_blobHeap[index] & 0x3F) << 8) + this->m_blobHeap[index + 1];
            case 4: return ((this->m_blobHeap[index] & 0x1F) << 24)
                            + (this->m_blobHeap[index + 1] << 16)
                            + (this->m_blobHeap[index + 2] << 8)
                            +  this->m_blobHeap[index + 3];
            default: return 0;
        }
    }

    u8 DLL::getBlobHeaderSize(u32 index) {
        if ((this->m_userStringsHeap[index] & 0x80) == 0x00)
            return 1;
        if ((this->m_userStringsHeap[index] & 0xC0) == 0x80)
            return 2;
        if ((this->m_userStringsHeap[index] & 0xE0) == 0xC0)
            return 4;

        return 0;
    }

    const char16_t* DLL::getUserString(u32 index) {
        if ((index >> 24) == 0x70)
            return reinterpret_cast<char16_t*>(&this->m_userStringsHeap[index & 0x00FFFFFF] + getBlobHeaderSize(index));

        return nullptr;
    }

    u8* DLL::getBlob(u32 index) {
        return &this->m_blobHeap[index + getBlobHeaderSize(index)];
    }

    u8* DLL::getData() {
        return this->m_dllData;
    }

    u32 DLL::getStackSize() {
        return this->m_optionalHeader->stackReserveSize;
    }

    std::string DLL::getFullMethodName(u32 methodToken) {
        auto memberRef = this->getMemberRefByMetadataToken(methodToken);
        auto typeRef = this->getTypeRefByIndex(INDEX_INDEX(memberRef->classIndex, MEMBER_REF_PARENT));
        auto assemblyRef = this->getAssemblyRefByIndex(INDEX_INDEX(typeRef->resolutionScopeIndex, RESOLUTION_SCOPE));

        auto assembly = this->getString(assemblyRef->nameIndex);
        auto nameSpace = this->getString(typeRef->typeNamespaceIndex);
        auto type = this->getString(typeRef->typeNameIndex);
        auto method = this->getString(memberRef->nameIndex);

        return "["s + assembly + "]"s + nameSpace + "."s + type + "::"s + method;
    }

    std::string DLL::decodeUserString(u32 token) {
        auto utf16String = this->getUserString(token);
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> conversion;
        return conversion.to_bytes(utf16String);
    }

    section_table_entry_t* DLL::getVirtualSection(u64 rva) {
        for (u8 section = 0; section < this->m_ntHeader->numSections; section++) {
            if (rva >= this->m_sectionTable[section]->virtualAddress
                && rva <  this->m_sectionTable[section]->virtualAddress + this->m_sectionTable[section]->virtualSize)
                return this->m_sectionTable[section];
        }

        return nullptr;
    }

    u16 DLL::findTypeDefWithMethod(u32 methodToken) {
        table_method_def_t *methodToFind = this->getMethodDefByMetadataToken(methodToken);

        if (methodToFind == nullptr)
            return 0;

        // Check all rows in the typedef table except the last one for if it contains the method
        for (u32 i = 0; i < this->m_numRows[TABLE_ID_METHODDEF] - 1; i++) {
            table_type_def_t* currTypeDef = reinterpret_cast<table_type_def_t*>(this->m_tildeTableData[TABLE_ID_TYPEDEF][i].base);
            table_type_def_t* nextTypeDef = reinterpret_cast<table_type_def_t*>(this->m_tildeTableData[TABLE_ID_TYPEDEF][i + 1].base);

            table_method_def_t* currMethodDef = this->getMethodDefByIndex(currTypeDef->methodListIndex);
            table_method_def_t* nextMethodDef = this->getMethodDefByIndex(nextTypeDef->methodListIndex);

            if (methodToFind >= currMethodDef && methodToFind < nextMethodDef)
                return i + 1;
        }

        // When the method hasn't been found in the previous rows, it has to be in the last one
        // since if it wouldn't exist at all, methodToFind would have been nullptr
        return this->m_numRows[TABLE_ID_METHODDEF];
    }

    table_class_layout_t* DLL::getClassLayoutOfType(table_type_def_t *typeDef) {
        for (u32 i = 0; i < this->m_numRows[TABLE_ID_CLASS_LAYOUT]; i++) {
            table_class_layout_t *currClassLayout = reinterpret_cast<table_class_layout_t*>(this->m_tildeTableData[TABLE_ID_CLASS_LAYOUT][i].base);

            if (reinterpret_cast<table_type_def_t*>(this->m_tildeTableData[TABLE_ID_TYPEDEF][currClassLayout->parentIndex].base) == typeDef)
                return currClassLayout;
        }

        return nullptr;
    }

    u32 DLL::getNumTableRows(u8 index) {
        if (index >= sizeof(this->m_numRows))
            return 0;

        return this->m_numRows[index];
    }

}