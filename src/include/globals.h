#include <stdint.h>

#include "sectable.h"
#include "sizes.h"

#ifndef _GLOBALS_H
#define _GLOBALS_H

typedef enum {
    OFF,
    ON,
} option_val;

typedef struct {
    char *seg;
    char *sec;
    char *out;
} dump_t;

typedef struct {
    option_val head;
    option_val no_head;
    option_val no_sec;
    option_val no_sym;
    uint32_t parse_only;
    option_val focus_seg;
    option_val silence;
    char* seg_name;
    struct {
        option_val dump;
        dump_t dump_info;
    } dumping;
} global_options;

extern size_kv cmd_sizes[CMDS_MAX_SIZE];

extern global_options opts;

extern char *macho_fn;
extern char *filename;

#endif
