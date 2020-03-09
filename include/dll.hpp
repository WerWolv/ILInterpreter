#pragma once

#include "types.hpp"
#include "file_headers.hpp"
#include "tables.hpp"
#include "method_state.hpp"

#include <string>
#include <stdio.h>
#include <cstring>
#include <vector>

namespace csharp {

#define OFFSET(base, offset) (reinterpret_cast<u8*>(base) + offset)
#define VRA_TO_OFFSET(section, rva) section->rawDataPointer + (rva - section->virtualAddress)
#define ALIGN(value, alignment) ((value) + alignment) & (~(alignment - 1))

    class DLL {
    public:
        DLL(std::string filePath) {
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
                    }
                }
            }
        }

        ~DLL() {
            delete[] this->m_dllData;
        }

        void validate() {
            if (this->m_dosHeader->magic != 0x5A4D) {
                printf("Invalid DOS Header!\n");
                exit(1);
            } else printf("Valid DOS Header!\n");

            if (this->m_ntHeader->magic != 0x00004550) {
                printf("Invalid NT Header!\n");
                exit(1);
            } else printf("Valid NT Header!\n");

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

        void execute(table_method_def_t *methodDef) {
            MethodState methodState;

            section_table_entry_t *ilHeaderSection = this->getVirtualSection(methodDef->rva);
            methodState.ILPtr = OFFSET(this->m_dllData, VRA_TO_OFFSET(ilHeaderSection, methodDef->rva + 1));
            methodState.programCounter = 0;

            u8 *ILPtr = methodState.ILPtr;
            while (true) {
                u8 *opcode = OFFSET(ILPtr, methodState.programCounter);

                switch (*opcode) {
                    case 0x00: // NOP
                        printf("%d : NOP\n", methodState.programCounter);
                        methodState.programCounter += 1;
                        break;
                    case 0x72: // NOP
                        printf("%d : LDRSTR\n", methodState.programCounter);
                        methodState.programCounter += 5;
                        break;
                    case 0x28: // CALL
                        printf("%d : CALL\n", methodState.programCounter);
                        methodState.programCounter += 5;
                        break;
                    case 0x2A: // RET
                        printf("%d : RET\n", methodState.programCounter);
                        methodState.programCounter += 1;
                        return;
                    default:
                        printf("%d : Unknown Opcode\n", methodState.programCounter);
                        return;
                }

            }
        }

        table_method_def_t* getMethodByToken(u32 methodToken) {
            switch (TABLE_ID(methodToken)) {
                case TABLE_ID_METHODDEF:
                    return reinterpret_cast<table_method_def_t*>(this->m_tildeTableData[TABLE_ID(methodToken)][TABLE_INDEX(methodToken) - 1].base);
            }

            return nullptr;
        }

        u32 getEntryMethodToken() {
            return this->m_crlRuntimeHeader->entryPointToken;
        }

        const char* getString(u32 index) {
            return reinterpret_cast<char*>(&this->m_stringsHeap[index]);
        }

    private:
        u8 *m_dllData;
        size_t m_fileSize;

        dos_header_t *m_dosHeader;
        dos_stub_t *m_dosStub;
        nt_header_t *m_ntHeader;
        optional_header_t *m_optionalHeader;
        std::vector<section_table_entry_t*> m_sectionTable;
        crl_runtime_header_t *m_crlRuntimeHeader;
        metadata_t m_metadata = { 0 };
        std::vector<stream_header_t*> m_streamHeaders;
        u32 m_rows[64] = { 0 };

        std::vector<std::vector<unspecified_table_t>> m_tildeTableData;
        u8 *m_stringsHeap;

        section_table_entry_t* getVirtualSection(u64 rva) {
            for (u8 section = 0; section < this->m_ntHeader->numSections; section++) {
                if (rva >= this->m_sectionTable[section]->virtualAddress
                 && rva <  this->m_sectionTable[section]->virtualAddress + this->m_sectionTable[section]->virtualSize)
                    return this->m_sectionTable[section];
            }

            return nullptr;
        }
    };

}