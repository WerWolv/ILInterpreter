#pragma once

namespace ili {

#define TABLE_ID(token) (token >> 24)
#define TABLE_INDEX(token) (token & 0x00FFFFFF)

#define TABLE_ID_METHODDEF      0x06
#define TABLE_ID_ASSEMBLYREF    0x23
#define TABLE_ID_TYPEREF        0x01
#define TABLE_ID_MEMBERREF      0x0A
#define TABLE_ID_MODULE         0x00

    typedef struct PACKED { // 0x06
        u32 rva;
        u16 implFlags;
        u16 flags;
        u16 nameIndex;
        u16 signatureIndex;
        u16 paramListIndex;
    } table_method_def_t;

    typedef struct PACKED { // 0x23
        u16 versionMajor;
        u16 versionMinor;
        u16 buildNumber;
        u16 revisionNumber;
        u32 flags;
        u16 publicKeyOrTokenIndex;
        u16 nameIndex;
        u16 cultureIndex;
        u16 hashValueIndex;
    } table_assembly_ref_t;

    typedef struct PACKED { // 0x01
        u16 resolutionScopeIndex;
        u16 typeNameIndex;
        u16 typeNamespaceIndex;
    } table_type_ref_t;

    typedef struct PACKED { // 0x0A
        u16 classIndex;
        u16 nameIndex;
        u16 signatureIndex;
    } table_member_ref_t;

    typedef struct PACKED { // 0x00
        u16 generation;
        u16 nameIndex;
        u16 mvId;
        u16 encId;
        u16 encBaseId;
    } table_module_t;

}