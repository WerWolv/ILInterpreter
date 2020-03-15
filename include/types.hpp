#pragma once

#include <cstdint>
#include <cstdbool>
#include <string>

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using s8  = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using namespace std::string_literals;

enum class Type : u8 {
    Invalid                 = 0,
    Int32                   = 1,
    Int64                   = 2,
    Native_int              = 4,
    Native_unsigned_int     = 8,
    F                       = 16,
    O                       = 32,
    Pointer                 = 64
};

enum class SignatureElementType : u8 {
    End,
    Void,
    Boolean,
    Char,
    I1,
    U1,
    I2,
    U2,
    I4,
    U4,
    I8,
    U8,
    R4,
    R8,
    String,
    Ptr,
    ByRef,
    ValueType,
    Class,
    Var,
    Array,
    GenericInst,
    TypedByRef,
    I,
    U,
    FuncPtr,
    Object,
    SzArray,
    MVar,
    CmodReqd,
    CmodOpt,
    Internal,
    Modifier,
    Sentinel,
    Pinned
};

static u8 getTypeSize(Type type) {
    switch (type) {
        case Type::Int32: return 4;
        case Type::Int64: return 8;
        case Type::Native_int: return 8;
        case Type::Native_unsigned_int: return 8;
        case Type::F: return 8;
        case Type::O: return 8;
        case Type::Pointer: return 8;
        default: return 0;
    }
}

#define PACKED __attribute__((packed))