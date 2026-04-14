#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "macho.h"

#ifndef _UTILS_H
#define _UTILS_H

#define READ_ERR_MSG "An error happened while reading the file!"
#define CMDS_MAX_SIZE 256


typedef struct {
    uint32_t key;
    uint32_t val;
} size_kv;

extern size_kv cmd_sizes[];

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} section_table;

typedef enum {
    SIZES_NOT_FOUND,
    SIZES_FOUND,
} sizes_status;

sizes_status sizes_get(uint32_t key, uint32_t *val);
void sizes_init(void);
void fetal(char* msg, FILE* f);
void handle_hstatus(header_status status, FILE* f);
void handle_loadcmd(loadcmd_status status, FILE* f);
void handle_loadcmd_sec(loadcmd_sec_status status, FILE* f);
void handle_loadcmd_symentry(loadcmd_sym_status, FILE* f);
void section_table_push(section_table *table, char* entry);

#endif
