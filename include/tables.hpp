#pragma once

namespace csharp {

#define TABLE_ID(token) (token >> 24)
#define TABLE_INDEX(token) (token & 0x00FFFFFF)

#define TABLE_ID_METHODDEF 0x06

    typedef struct PACKED {
        u32 rva;
        u16 implFlags;
        u16 flags;
        u16 nameIndex;
        u16 signatureIndex;
        u16 paramListIndex;
    } table_method_def_t;

}