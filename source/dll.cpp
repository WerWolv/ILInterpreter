#include "dll.hpp"

#include "types.hpp"
#include "file_headers.hpp"
#include "tables.hpp"

#include <string>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <codecvt>
#include <locale>

namespace ili {

#define OFFSET(base, offset) (reinterpret_cast<u8*>(base) + offset)
#define VRA_TO_OFFSET(section, rva) section->rawDataPointer + (rva - section->virtualAddress)
#define ALIGN(value, alignment) ((value) + alignment) & (~(alignment - 1))

    DLL::DLL(std::string filePath) {
        FILE *dllFile = fopen(filePath.c_str(), "rb");

        if (dllFile == nullptr) {
            printf("Cannot open file %s!", filePath.c_str());
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
                currentDataPtr += 2 * sizeof(u32) + ALIGN(strlen(currHeader->name), 4);
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
                            this->m_rows[i] = *reinterpret_cast<u32*>(currentDataPtr);
                            currentDataPtr += sizeof(u32);
                        }
                    }

                    for (u8 i = 0; i < 64; i++) {
                        u32 count = this->m_rows[i];

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
                }
            }
        }
    }

    DLL::~DLL() {
        delete[] this->m_dllData;
    }

    void DLL::validate() {
        if (this->m_dosHeader->magic != 0x5A4D) {
            printf("Invalid DOS Header!\n");
            exit(1);
        } else printf("Valid DOS Header!\n");

        if (this->m_ntHeader->magic != 0x00004550) {
            printf("Invalid NT Header!\n");
            exit(1);
        } else printf("Valid NT Header!\n");

        printf("Stack size: %lx\n", this->getStackSize());

        if (this->m_crlRuntimeHeader->headerSize != sizeof(crl_runtime_header_t)) {
            printf("Invalid CLR Header!\n");
            exit(1);
        } else printf("Valid CLR Header!\n");

        printf("Runtime version: %d.%d\n", this->m_crlRuntimeHeader->runtimeVersionMajor, this->m_crlRuntimeHeader->runtimeVersionMinor);
        printf("Entrypoint Token: %x\n", this->m_crlRuntimeHeader->entryPointToken);

        if (this->m_metadata.magic != 0x424A5342) {
            printf("Invalid Metadata Header!\n");
            exit(1);
        } else printf("Valid Metadata Header!\n");

        printf(".NET Framework version: %s\n", this->m_metadata.version);

        for (u8 stream = 0; stream < this->m_metadata.streams; stream++) {
            printf("Found Stream: %s\n", this->m_streamHeaders[stream]->name);

            if (std::string(this->m_streamHeaders[stream]->name) == "#~") {
                for (u8 i = 0; i < 64; i++)
                    if (this->m_rows[i] != 0)
                        printf("  Table 0x%X: %u entries\n", i, this->m_rows[i]);
            }
        }
    }

    table_method_def_t* DLL::getMethodDefByToken(u32 methodToken) {
        if (TABLE_ID(methodToken) == TABLE_ID_METHODDEF)
            return reinterpret_cast<table_method_def_t*>(this->m_tildeTableData[TABLE_ID(methodToken)][TABLE_INDEX(methodToken) - 1].base);
        else return nullptr;
    }

    table_member_ref_t* DLL::getMemberRefByToken(u32 memberToken) {
        if (TABLE_ID(memberToken) == TABLE_ID_MEMBERREF)
            return reinterpret_cast<table_member_ref_t*>(this->m_tildeTableData[TABLE_ID(memberToken)][TABLE_INDEX(memberToken) - 1].base);
        else return nullptr;
    }

    table_type_ref_t* DLL::getTypeRefByToken(u32 typeToken) {
        return reinterpret_cast<table_type_ref_t*>(this->m_tildeTableData[TABLE_ID_TYPEREF][(typeToken >> 3) - 1].base);
    }

    table_assembly_ref_t* DLL::getAssemblyRefByToken(u32 assemblyToken) {
        return reinterpret_cast<table_assembly_ref_t*>(this->m_tildeTableData[TABLE_ID_ASSEMBLYREF][(assemblyToken >> 2) - 1].base);
    }

    u32 DLL::getEntryMethodToken() {
        return this->m_crlRuntimeHeader->entryPointToken;
    }

    const char* DLL::getString(u32 index) {
        return reinterpret_cast<char*>(&this->m_stringsHeap[index]);
    }

    const char16_t* DLL::getUserString(u32 index) {
        if ((index >> 24) == 0x70)
            return reinterpret_cast<char16_t*>(&this->m_userStringsHeap[index & 0x00FFFFFF] + 1);

        return nullptr;
    }

    u8* DLL::getData() {
        return this->m_dllData;
    }

    u32 DLL::getStackSize() {
        return this->m_optionalHeader->stackReserveSize;
    }

    std::string DLL::getMethodSignature(u32 methodToken) {
        auto memberRef = this->getMemberRefByToken(methodToken);
        auto typeRef = this->getTypeRefByToken(memberRef->classIndex);
        auto assemblyRef = this->getAssemblyRefByToken(typeRef->resolutionScopeIndex);

        auto assembly = this->getString(assemblyRef->nameIndex);
        auto nameSpace = this->getString(typeRef->typeNamespaceIndex);
        auto type = this->getString(typeRef->typeNameIndex);
        auto method = this->getString(memberRef->nameIndex);

        return assembly + "::"s + nameSpace + "."s + type + "."s + method;
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

}