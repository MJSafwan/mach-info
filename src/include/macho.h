#include <stdio.h>
#include <stdint.h>

#ifndef _MACHO_H
#define _MACHO_H

#define MAGIC64 0xfeedfacf
#define MAGIC64_FAT 0xbebafeca 


#define    CMD_ERR 0x0
#define    CMD_SEG64  0x19
#define    CMD_MAIN  0x80000028
#define    CMD_SYMTABLE  0x2

typedef uint32_t loadcmd_type;

typedef struct {
    uint32_t magic;
    uint32_t cpu_type;
    uint32_t cpu_subtype;
    uint32_t filetype;
    uint32_t num_of_load_cmds;
    uint32_t size_of_load_cmds;
    uint32_t flags;
    uint32_t reserved;
} header_macho;

typedef struct {
    uint32_t type;
    uint32_t size;
    char name[16];
    uint64_t address;
    uint64_t address_size;
    uint64_t file_offset;
    uint64_t filesize;
    uint32_t max_protect;
    uint32_t init_protect;
    uint32_t num_of_sec;
    uint32_t flags32;
} loadcmd_seg64;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint64_t offset;
    uint64_t stack_size;
} loadcmd_main;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t symbol_offset;
    uint32_t number_of_symbols;
    uint32_t string_offset;
    uint32_t string_size;
} loadcmd_symtable;

typedef struct {
    char sec_name[16];
    char seg_name[16];
    uint64_t addr;
    uint64_t size;
    uint32_t offset;
    uint32_t align;
    uint32_t reloff;
    uint32_t nreloc;
    uint32_t flags;
    uint32_t reserved1;
    uint32_t reserved2;
    uint32_t reserved3;
} loadcmd_sec64;

typedef struct {
    uint32_t n_strx;
    uint8_t  n_type;       
    uint8_t  n_sect;       
    uint16_t n_desc;       
    uint64_t n_value;
} loadcmd_symentry;

typedef struct {
    uint32_t type;
    uint32_t size;
    union {
        void *as_cmd;
        loadcmd_seg64 as_cmd_seg64;
        loadcmd_main  as_cmd_main;
        loadcmd_symtable as_cmd_symtable;
    };
} loadcmd;

typedef enum {
    HEADER_FINE,
    HEADER_ERR_READ,
    HEADER_ERR_NOT_MACHO64,
    HEADER_ERR_FAT,
} header_status;

typedef enum {
    LOADCMD_FINE,
    LOADCMD_ERR_READ,
    LOADCMD_ERR_UNKNOWN_CMD,
} loadcmd_status;

typedef enum {
    LOADCMD_SEC_FINE,
    LOADCMD_SEC_ERR_READ,
} loadcmd_sec_status;

typedef enum {
    LOADCMD_SYM_FINE,
    LOADCMD_SYM_ERR_READ,
} loadcmd_sym_status;

header_status header_get(header_macho* h, FILE* f);
loadcmd_status loadcmd_get(loadcmd *lcmd, FILE* f);
loadcmd_sec_status loadcmd_sec_get(loadcmd_sec64 *sec, FILE* f);
loadcmd_sym_status loadcmd_symentry_get(loadcmd_symentry *entry, FILE* f);

#endif
