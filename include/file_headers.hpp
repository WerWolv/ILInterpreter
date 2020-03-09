#pragma once

#include <types.hpp>

typedef struct PACKED {
    u16 magic;              // MZ
    u8 unused[0x3C];
    u16 peHeaderPointer;    // Offset 0x3C
} dos_header_t;
static_assert(sizeof(dos_header_t) == 0x40, "dos_header_t size invalid!");

typedef struct PACKED {
    u8 data[0x40];
} dos_stub_t;
static_assert(sizeof(dos_stub_t) == 0x40, "dos_stub_t size invalid!");

typedef struct PACKED {
    u32 magic;              // PE\0\0 (0x00004550)
    u16 machine;
    u16 numSections;
    u32 timeDateStamp;
    u32 symbolTablePointer;
    u32 numSymbolTables;
    u16 optionalHeaderSize;
    u16 characteristics;
} nt_header_t;
static_assert(sizeof(nt_header_t) == 0x18, "nt_header_t size invalid!");

typedef struct PACKED {
    u32 rva;
    u32 size;
} table_t;
static_assert(sizeof(table_t) == 0x08, "table_t size invalid!");

typedef struct PACKED {
    u16 magic;
    u8 linkerVersionMajor;
    u8 linkerVersionMinor;
    u32 codeSize;
    u32 bssSize;
    u32 dataSize;
    u32 entryPointRVA;
    u32 codeBaseRVA;
    u32 dataBaseRVA;

    u32 imageBase;
    u32 sectionAlignment;
    u32 fileAlignment;
    u16 osVersionMajor;
    u16 osVersionMinor;
    u16 imageVersionMajor;
    u16 imageVersionMinor;
    u16 subsystemVersionMajor;
    u16 subsystemVersionMinor;
    u32 win32VersionValue;
    u32 imageSize;
    u32 headersSize;
    u32 checksum;
    u16 subsystem;
    u16 dllCharacteristics;
    u32 stackReserveSize;
    u32 stackCommitSize;
    u32 heapReserveSize;
    u32 heapCommitSize;
    u32 loaderFlags;
    u32 numRvaAndSizes;

    table_t exportTable;
    table_t importTable;
    table_t resourceTable;
    table_t exceptionTable;
    table_t certificateTable;
    table_t baseRelocationTable;
    table_t debug;
    table_t architectureData;
    table_t globalPointer;
    table_t tslTable;
    table_t loadConfigTable;
    table_t boundImport;
    table_t importAddressTable;
    table_t delayImportAddressTable;
    table_t crlRuntimeHeader;
    u64 unused;
} optional_header_t;
static_assert(sizeof(optional_header_t) == 0xE0, "optional_header_t size invalid!");

typedef struct PACKED {
    char name[8];
    u32 virtualSize;
    u32 virtualAddress;
    u32 rawDataSize;
    u32 rawDataPointer;
    u32 relocationsPointer;
    u32 lineNumbersPointer;
    u16 numRelocations;
    u16 numLineNumbers;
    u32 characteristics;
} section_table_entry_t;
static_assert(sizeof(section_table_entry_t) == 0x28, "section_table_entry_t size invalid!");

typedef struct PACKED {
    u32 headerSize;
    u16 runtimeVersionMajor;
    u16 runtimeVersionMinor;
    table_t metaData;
    u32 flags;
    u32 entryPointToken;
    u64 resources;
    u64 strongNameSignature;
    u64 codeManagerTable;
    u64 vTableFixups;
    u64 exportAddressTableJumps;
    u64 managedNativeHeader;
} crl_runtime_header_t;
static_assert(sizeof(crl_runtime_header_t) == 0x48, "crl_runtime_header_t size invalid!");

typedef struct PACKED {
    u32 offset;
    u32 size;
    char name[32];
} stream_header_t;

typedef struct PACKED {
    u32 magic;
    u16 versionMajor;
    u16 versionMinor;
    u32 reserved;
    u32 length;
    char version[0xFF];
    u16 flags;
    u16 streams;
} metadata_t;

typedef struct PACKED {
    u32 reserved_x00;
    u8 versionMajor;
    u8 versionMinor;
    u8 heapSize;
    u8 reserved_x07;
    u64 valid;
    u64 sorted;
} tilde_stream_t;

typedef struct PACKED {
    u8 *base;
    size_t size;
} unspecified_table_t;



static constexpr u8 getMetadataTableSize(u8 index) {
    // TODO: Some of these values depend on if a table/heap has more than 2^16 entries
    // TODO: For now, assume we don't reach that limit
    constexpr u8 table[64] = {
            10, 6, 14, 0, 6, 0, 14, 0,
            6, 0, 6, 0, 6, 0, 0, 0,
            0, 2, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            22, 0, 0, 20
    };

    if (index >= sizeof(table))
        return 0;

    return table[index];
}