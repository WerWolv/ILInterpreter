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

        table_method_def_t* getMethodDefByMetadataToken(u32 methodToken);
        table_member_ref_t* getMemberRefByMetadataToken(u32 memberToken);
        table_method_def_t* getMethodDefByIndex(u32 index);
        table_type_def_t* getTypeDefByIndex(u32 index);
        table_type_ref_t* getTypeRefByIndex(u32 index);
        table_assembly_ref_t* getAssemblyRefByIndex(u32 index);
        table_field_t* getFieldByIndex(u32 index);

        u32 getEntryMethodToken();

        const char* getString(u32 index);
        const char16_t* getUserString(u32 index);
        u8 *getBlob(u32 index);

        u8* getData();

        u32 getStackSize();

        section_table_entry_t* getVirtualSection(u64 rva);

        std::string getFullMethodName(u32 methodToken);
        std::string decodeUserString(u32 token);

        u16 findTypeDefWithMethod(u32 methodToken);
        table_class_layout_t* getClassLayoutOfType(table_type_def_t *typeDef);

        u32 getBlobSize(u32 index);
        u8 getBlobHeaderSize(u32 index);

        u32 getNumTableRows(u8 index);

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
        u32 m_numRows[64] = { 0 };

        std::vector<std::vector<unspecified_table_t>> m_tildeTableData;
        u8 *m_stringsHeap;
        u8 *m_userStringsHeap;
        u8 *m_blobHeap;
    };

}