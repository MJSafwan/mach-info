#include "utils.h"

void section_table_push(section_table *table, char* entry) {
    if (table->count == table->capacity) {
        table->capacity = table->capacity == 0 ? 1 : table->capacity * 2;
        table->items = realloc(table->items, table->capacity * sizeof(entry));
        assert(table->items);
    }
    
    table->items[table->count++] = entry;
}

sizes_status sizes_get(uint32_t key, uint32_t *val) {
    for (size_t i = 0; i < CMDS_MAX_SIZE;++i) {
        size_kv kv = cmd_sizes[i];
        if (kv.key == 0) break;
        if (kv.key == key) {
            val[0] = kv.val;
            return SIZES_FOUND;
        }
    }
    return SIZES_NOT_FOUND;
}

void sizes_init(void) {
    cmd_sizes[0] = (size_kv) {CMD_SEG64, sizeof(loadcmd_seg64)};
    cmd_sizes[1] = (size_kv) {CMD_MAIN, sizeof(loadcmd_main)};
    cmd_sizes[2] = (size_kv) {CMD_SYMTABLE, sizeof(loadcmd_symtable)};
}

void fetal(char* msg, FILE* f) {
    fprintf(stderr, "[ERROR] %s\n", msg);
    fclose(f);
    exit(1);
}

void handle_hstatus(header_status status, FILE* f) {
    switch (status) {
        case HEADER_ERR_READ:
            fetal(READ_ERR_MSG, f);
            break;
        case HEADER_ERR_NOT_MACHO64:
            fetal("The file is not a 64-bit Mach-O file!", f);
            break;
        case HEADER_ERR_FAT:
            fetal("Cannot handle fat Mach-O files yet!", f);
            break;
        case HEADER_FINE:
        default:
            break;
    }
}


void handle_loadcmd(loadcmd_status status, FILE* f) {
    switch(status) {
        case LOADCMD_ERR_READ:
            fetal(READ_ERR_MSG, f);
            break;
        case LOADCMD_ERR_UNKNOWN_CMD:
            /* Do nothing for now. */
            break;
        case LOADCMD_FINE:
        default:
            break;
    }
}
