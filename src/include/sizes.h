#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef _SIZES_H
#define _SIZES_H

#define CMDS_MAX_SIZE 256

typedef struct {
    uint32_t key;
    uint32_t val;
} size_kv;

extern size_kv cmd_sizes[];

typedef enum {
    SIZES_NOT_FOUND,
    SIZES_FOUND,
} sizes_status;

sizes_status sizes_get(uint32_t key, uint32_t *val);
void sizes_init(void);

#endif
