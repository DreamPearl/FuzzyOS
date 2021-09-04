#pragma once

// Min GDT_TABLE_SIZE:    5
// Max GDT_TABLE_SIZE: 8192

// START_ENSURE_SAME_gdt_asm
#define GDT_STD_SELECTOR_NULL      0
#define GDT_STD_SELECTOR_KERNEL_CS 1
#define GDT_STD_SELECTOR_KERNEL_DS 2
#define GDT_STD_SELECTOR_ABS16_CS  3
#define GDT_STD_SELECTOR_ABS16_DS  4
#define GDT_STD_SELECTOR_ABS32_DS  6
#define GDT_STD_SIZE 5 // FIX
// Entries on and after GDT_STD_SIZE are reserved for applications
// and are in CS, DS order for each app.
// END_ENSURE_SAME_gdt_asm

#pragma pack(push, 1)
struct GDTReference {
    unsigned short size;
    unsigned int base_address;
};
struct GDTEntry {
    unsigned short limit0;
    unsigned short base0;
    unsigned char base1;
    unsigned char access_byte;
    unsigned char flags_limit1;
    unsigned char base2;
};
#pragma pack(pop)

#define GDT_KERNEL_CS (GDT_STD_SELECTOR_KERNEL_CS*sizeof(struct GDTEntry))

int get_gdt_baseaddress(struct GDTEntry gdt_table[], unsigned int table_size, int entry_id);

void populate_gdt_entry(struct GDTEntry *entry,
    unsigned int base,
    unsigned int limit,  // 20 bits
    unsigned char flags, //  4 bits
    unsigned char access_byte
    );