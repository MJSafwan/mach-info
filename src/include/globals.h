#include <stdint.h>
#include "utils.h"

#ifndef _GLOBALS_H
#define _GLOBALS_H

typedef enum {
    OFF,
    ON,
} option_val;

typedef struct {
    option_val head;
    option_val no_head;
    option_val no_sec;
    option_val no_sym;
    uint32_t parse_only;
    option_val focus_seg;
    char* seg_name;
} global_options;

extern size_kv cmd_sizes[CMDS_MAX_SIZE];
extern FILE *f;
extern section_table stable;

extern global_options opts;

extern char *macho_fn;
extern char *filename;

#endif
