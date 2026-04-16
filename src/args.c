#include "args.h"
#include "globals.h"
#include "macho.h"

void parse_args(int argc, char** argv) {
    char* buff = NULL;
    do {
        buff = skip(argc, argv);
        if (buff == NULL) break;

        if (buff[0] != '-') {
            if (macho_fn != NULL) {
                fprintf(stderr, "\nTwo files detected!\n");
                fprintf(stderr, "You should only supply one file.\n");
                fprintf(stderr, HELP, filename);
                exit(1);
            }
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

        if (strcmp(buff, "-dump") == 0) {
            dump_t d = {0};
            d.seg = skip(argc, argv);
            d.sec = skip(argc, argv);
            if (!d.seg || !d.sec) {
                fprintf(stderr, "\nUsage: %s -dump <seg64 name> <sec name> <filename>\n", filename);
                exit(1);
            }
            opts.dumping.dump = ON;
            opts.dumping.dump_info = d;
            opts.silence = ON;

            continue;
        }
        
        UNKNOWN(buff, filename);
    } while (buff != NULL);
}
