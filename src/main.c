#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "args.h"
#include "contents.h"
#include "globals.h"
#include "macho.h"
#include "parse.h"
#include "print.h"
#include "handler.h"
#include "sizes.h"
#include "sectable.h"

size_kv cmd_sizes[CMDS_MAX_SIZE] = {0};
global_options opts = {0};

char *macho_fn = NULL;
char *filename = NULL;

int main(int argc, char **argv) {
    filename = skip(argc, argv);
    parse_args(argc, argv);
    if (macho_fn == NULL) {
        fprintf(stderr, HELP, filename);
        exit(1);
    }
    
    FILE *file = fopen(macho_fn, "rb");
    if (file == NULL) {
        perror(macho_fn);
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    rewind(file);

    char *contents = malloc(file_size);
    if (contents == NULL) {
        perror("malloc");
        return 1;
    }

    if (fread(contents, file_size, 1, file) != 1) {
        fetal(READ_ERR_MSG);
    }

    fclose(file);

    contents_t c = {
        .items = contents,
        .size = file_size,
        .offset = 0,
    };

    sizes_init();

    header_macho m = {0};
    handle_hstatus(header_get(&m, &c));
    parse_head(m);

    section_table stable = {0};
    for (size_t i = 0; i < m.num_of_load_cmds; ++i) {
        loadcmd cmd = {0};
        loadcmd_status s = loadcmd_get(&cmd, &c);
        handle_loadcmd(s);
        if (s == LOADCMD_ERR_UNKNOWN_CMD) continue;
        
        parse_cmd(cmd, i, &c, &stable);
    }

    section_table_free(&stable);
    free(contents);
    return 0;
}
