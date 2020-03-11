#pragma once

#include "types.hpp"
#include "file_headers.hpp"
#include "tables.hpp"

#include <string>
#include <stdio.h>
#include <cstring>
#include <vector>

namespace ili {

#define OFFSET(base, offset) (reinterpret_cast<u8*>(base) + offset)
#define VRA_TO_OFFSET(section, rva) section->rawDataPointer + (rva - section->virtualAddress)
#define ALIGN(value, alignment) ((value) + alignment) & (~(alignment - 1))

    class DLL {
    public:
        DLL(std::string filePath);
        ~DLL();

        void validate();

        table_method_def_t* getMethodDefByToken(u32 methodToken);
        table_member_ref_t* getMemberRefByToken(u32 memberToken);
        table_type_ref_t* getTypeRefByToken(u32 typeToken);
        table_assembly_ref_t* getAssemblyRefByToken(u32 assemblyToken);

        u32 getEntryMethodToken();

        const char* getString(u32 index);

        const char16_t* getUserString(u32 index);

        u8* getData();

        u32 getStackSize();

        section_table_entry_t* getVirtualSection(u64 rva);

        std::string getMethodSignature(u32 methodToken);
        std::string decodeUserString(u32 token);

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
        u8 *m_userStringsHeap;
    };

}