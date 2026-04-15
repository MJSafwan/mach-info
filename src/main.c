#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "macho.h"
#include "utils.h"
#include "print.h"
#include "globals.h"
#include "args.h"
#include "parse.h"

size_kv cmd_sizes[CMDS_MAX_SIZE] = {0};

FILE *f = NULL;
section_table stable = {0};

global_options opts = {0};

char *macho_fn = NULL;
char *filename = NULL;

void free_stable(void) {
    for (size_t i = 0; i < stable.count; ++i) {
        free(stable.items[i]);
    }
    free(stable.items);
    stable.items = NULL;
}

int main(int argc, char **argv) {
    filename = skip(argc, argv);
    parse_args(argc, argv);
    if (macho_fn == NULL) {
        fprintf(stderr, HELP, filename);
        exit(1);
    }
    
    f = fopen(macho_fn, "rb");
    if (f == NULL) {
        perror(macho_fn);
        return 1;
    }

    sizes_init();
    header_macho m = {0};
    handle_hstatus(header_get(&m, f), f);
    parse_head(m);

    for (size_t i = 0; i < m.num_of_load_cmds; ++i) {
        loadcmd cmd = {0};
        loadcmd_status s = loadcmd_get(&cmd, f);
        handle_loadcmd(s, f);
        if (s == LOADCMD_ERR_UNKNOWN_CMD) continue;
        
        parse_cmd(cmd, i);
    }

    free_stable();
    fclose(f);
    return 0;
}
