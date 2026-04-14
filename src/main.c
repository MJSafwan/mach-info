#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "macho.h"
#include "utils.h"
#include "print.h"
#define skip(c, v) (c>0 ? (c--, v = &v[1], v[-1]) : NULL)
#define USAGE "Usage: %s <filename>\n"
#define CONFLICT(f) fprintf(stderr, "\nConflicting commands.\n");\
                   fprintf(stderr, HELP, f);\
                   exit(1);
#define UNKNOWN(o, f) fprintf(stderr, "\nUnknown option '%s'\n", o);\
                      fprintf(stderr, HELP, f);\
                      exit(1);

#define HELP "\nMach-O file parser\n\n"\
             USAGE\
             "\nOptions:\n"\
             "-h or -help              : Displays this\n"\
             "-head                    : Only shows header\n"\
             "-seg64                   : Only shows segments\n"\
             "-symtab                  : Only shows the symbol table\n"\
             "-no-head                 : Hides the header\n"\
             "-no-sec                  : Hides sections\n"\
             "-no-sym                  : Hides the symbol table entries\n"\
             "-seg64-info <seg64 name> : Shows information about <seg64 name>\n"\

size_kv cmd_sizes[CMDS_MAX_SIZE] = {0};

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

static global_options opts = {0};

static char *macho_fn = NULL;
static char *filename = NULL;

typedef enum {
    MATCH,
    NO_MATCH,
} parse_cmp_status;

int can_print(loadcmd cmd) {
    if (opts.parse_only != 0) {
        if (cmd.type == opts.parse_only) return 1;
        return 0;
    }

    if (opts.focus_seg == ON) {
        if (cmd.type == CMD_SEG64) {
            if (strcmp(cmd.as_cmd_seg64.name, opts.seg_name) == 0) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

static parse_cmp_status parse_cmp(uint32_t type1, uint32_t type2) {
    if (type1 != type2) return NO_MATCH;
    return MATCH;
}

void parse_args(int argc, char** argv) {
    char* buff = NULL;
    do {
        buff = skip(argc, argv);
        if (buff == NULL) break;

        if (buff[0] != '-') {
            macho_fn = buff;
            continue;
        }

        if (strcmp(buff, "-h") == 0) {
            printf(HELP, filename);
            exit(0);
        }

        if (strcmp(buff, "-help") == 0) {
            printf(HELP, filename);
            exit(0);
        }

        if (strcmp(buff, "-head") == 0) {
            if (opts.no_head == ON) {
                CONFLICT(filename);
            }
            opts.head = ON;
            continue;
        }
        if (strcmp(buff, "-no-head") == 0) {
            if (opts.head == ON) {
                CONFLICT(filename);
            }
            opts.no_head = ON;
            continue;
        }
        
        if (strcmp(buff, "-seg64") == 0) {
            if (opts.parse_only != 0) {
                CONFLICT(filename);
            }
            opts.no_head = ON;
            opts.parse_only = CMD_SEG64;
            continue;
        }

        if (strcmp(buff, "-seg64-info") == 0) {
            if (opts.parse_only != 0) {
                CONFLICT(filename);
            }
            opts.no_head = ON;
            char *seg_name = skip(argc, argv);
            opts.focus_seg = ON;
            opts.seg_name = seg_name;
            if (seg_name == NULL) {
                printf(HELP, filename);
                exit(1);
            }
            continue;
        }

        if (strcmp(buff, "-symtab") == 0) {
            if (opts.parse_only != 0) {
                CONFLICT(filename);
            }
            opts.no_head = ON;
            opts.parse_only = CMD_SYMTABLE;
            continue;
        }

        if (strcmp(buff, "-no-sec") == 0) {
            opts.no_sec = ON;
            continue;
        }

        if (strcmp(buff, "-no-sym") == 0) {
            opts.no_sym = ON;
            continue;
        }
        
        UNKNOWN(buff, filename);
    } while (buff != NULL);
}

int main(int argc, char **argv) {
    filename = skip(argc, argv);
    parse_args(argc, argv);
    if (macho_fn == NULL) {
        fprintf(stderr, HELP, filename);
        exit(1);
    }

    
    FILE *f = fopen(macho_fn, "rb");
    if (f == NULL) {
        perror(macho_fn);
        return 1;
    }
    sizes_init();
    header_macho m = {0};
    
    handle_hstatus(header_get(&m, f), f);

    if (opts.no_head == OFF) {
        if (opts.head == ON) {
            print_header(m);
            return 0;
        }
        print_header(m);
        printf("\n----Load Commands----\n");
    }

    section_table stable = {0};
    for (size_t i = 0; i < m.num_of_load_cmds; ++i) {
        loadcmd cmd = {0};
        loadcmd_status s = loadcmd_get(&cmd, f);
        handle_loadcmd(s, f);
        if (s == LOADCMD_ERR_UNKNOWN_CMD) continue;
        
        if (can_print(cmd)) {
                printf("\n\nLoad Command %zu:\n\n", i);
                print_loadcmd(cmd);
        }

        if (parse_cmp(cmd.type, CMD_SEG64) == MATCH) {
            uint32_t nsec = cmd.as_cmd_seg64.num_of_sec;
            for (size_t k = 0; k < nsec; k++) {
                loadcmd_sec64 sec64 = {0};
                handle_loadcmd_sec(loadcmd_sec_get(&sec64, f), f);
                if (can_print(cmd)) {
                    if (opts.no_sec == OFF) {
                        printf("\nSection %zu.%zu:\n\n", i, k);
                        print_loadcmd_sec(sec64);
                    }
                }
                section_table_push(&stable, strdup(sec64.sec_name));
            }
        }

        if (parse_cmp(cmd.type, CMD_SYMTABLE) == MATCH) {
            uint32_t nsym = cmd.as_cmd_symtable.number_of_symbols;
            fpos_t pos;

            (void) fgetpos(f, &pos);

            char *string_table = alloca(cmd.as_cmd_symtable.string_size);
            (void) fseek(f, cmd.as_cmd_symtable.string_offset, SEEK_SET);
            if (fread(string_table, cmd.as_cmd_symtable.string_size, 1, f) != 1) {
                (void) fsetpos(f, &pos);
                break;
            }

            (void) fseek(f, cmd.as_cmd_symtable.symbol_offset, SEEK_SET);

            for (size_t k = 0; k < nsym; k++) {
                loadcmd_symentry entry = {0};
                handle_loadcmd_symentry(loadcmd_symentry_get(&entry, f), f);
                if (can_print(cmd)) {
                    if (opts.no_sym == OFF) {
                        printf("\n\nSymbol %zu:\n\n", k);
                        print_loadcmd_symbol(entry, string_table, stable);
                    }
                }
            }

            for (size_t i = 0; i < stable.count; ++i) {
                free(stable.items[i]);
            }
            free(stable.items);

            (void) fsetpos(f, &pos);
        }
    }
    fclose(f);
    return 0;
}
