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
    F                       = 8,
    O                       = 16,
    Pointer                 = 32
};

static u8 getTypeSize(Type type) {
    switch (type) {
        case Type::Int32: return 4;
        case Type::Int64: return 8;
        case Type::Native_int: return 4;
        case Type::F: return 8;
        case Type::O: return 4;
        case Type::Pointer: return 8;
        default: return 0;
    }
}

#define PACKED __attribute__((packed))