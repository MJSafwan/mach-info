#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "sizes.h"
#include "macho.h"

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

