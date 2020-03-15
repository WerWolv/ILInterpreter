#pragma once

namespace ili {

#define TABLE_ID(token) (token >> 24)
#define TABLE_INDEX(token) (token & 0x00FFFFFF)

#define TABLE_ID_METHODDEF      0x06
#define TABLE_ID_ASSEMBLYREF    0x23
#define TABLE_ID_TYPEREF        0x01
#define TABLE_ID_TYPEDEF        0x02
#define TABLE_ID_MEMBERREF      0x0A
#define TABLE_ID_CLASS_LAYOUT   0x0F
#define TABLE_ID_MODULE         0x00
#define TABLE_ID_FIELD          0x04

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

    typedef struct PACKED { // 0x02
        u32 flags;
        u16 typeNameIndex;
        u16 typeNamespaceIndex;
        u16 extendsIndex;
        u16 fieldListIndex;
        u16 methodListIndex;
    } table_type_def_t;

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

    typedef struct PACKED { // 0x0F
        u16 packingSize;
        u32 classSize;
        u16 parentIndex;
    } table_class_layout_t;

    typedef struct PACKED { // 0x04
        u16 flags;
        u16 nameIndex;
        u16 signatureIndex;
    } table_field_t;

#define TYPE_DEF_OR_REF 2
#define HAS_CONSTANT 2
#define HAS_CUSTOM_ATTRIBUTE 5
#define HAS_FIELD_MARSHALL 1
#define HAS_DECL_SECURITY 2
#define MEMBER_REF_PARENT 3
#define HAS_SEMANTICS 1
#define METHOD_DEF_OR_REF 1
#define MEMBER_FORWARDED 1
#define IMPLEMENTATION 2
#define CUSTOM_ATTRIBUTE_TYPE 3
#define RESOLUTION_SCOPE 2
#define TYPE_OR_METHOD_DEF 1

#define INDEX_TAG(index, tag_type) index & (0xFFFFFFFF << tag_type)
#define INDEX_INDEX(index, tag_type) (index >> tag_type)

}