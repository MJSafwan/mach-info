#include "parse.h"
#include "globals.h"
#include "print.h"
#include "handler.h"

static int can_print(loadcmd cmd) {
    if (opts.silence == ON) return 0;
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

static void parse_cmd_seg64(loadcmd cmd, contents_t *c, section_table *stable) {
    uint32_t nsec = cmd.as_cmd_seg64.num_of_sec;
    for (size_t k = 0; k < nsec; k++) {
        loadcmd_sec64 sec64 = {0};
        handle_loadcmd(loadcmd_sec_get(&sec64, c));
        if (can_print(cmd)) {
            if (opts.no_sec == OFF) {
                printf("\nSection %zu:\n\n", k);
                print_loadcmd_sec(sec64);
            }
        }

        if (opts.dumping.dump == ON) {
            if (strcmp(opts.dumping.dump_info.seg, sec64.seg_name) != 0) {
                continue;
            }
            if (strcmp(opts.dumping.dump_info.sec, sec64.sec_name) != 0) {
                continue;
            }


            contents_t s = {
                .items = &c->items[sec64.offset],
                .size = sec64.size,
                .offset = 0,
            };

            if (fwrite(s.items, sec64.size, 1, stdout) != 1) {
                perror("stdout");
                exit(1); 
            }

            exit(1);
        }

        section_table_push(stable, strdup(sec64.sec_name));
    }
}

static void parse_cmd_symtable(loadcmd cmd, contents_t *c, section_table *stable) {
    
    uint32_t nsym = cmd.as_cmd_symtable.number_of_symbols;

    contents_t string_c = {
        .items = &c->items[cmd.as_cmd_symtable.string_offset],
        .size = cmd.as_cmd_symtable.string_size,
        .offset = 0,
    };

    contents_t sym_c = {
        .items = &c->items[cmd.as_cmd_symtable.symbol_offset],
        .size = nsym * sizeof(loadcmd_symentry),
        .offset = 0,
    };

    for (size_t k = 0; k < nsym; k++) {
        loadcmd_symentry entry = {0};
        handle_loadcmd(loadcmd_symentry_get(&entry, &sym_c));
        if (can_print(cmd)) {
            if (opts.no_sym == OFF) {
                printf("\n\nSymbol %zu:\n\n", k);
                print_loadcmd_symbol(entry, &string_c, stable[0]);
            }
        }
    }
}

void parse_cmd(loadcmd cmd, size_t i, contents_t *c, section_table *stable) {

    if (can_print(cmd)) {
        printf("\n\nLoad Command %zu:\n\n", i);
        print_loadcmd(cmd);
    }

    switch (cmd.type) {
        case CMD_SEG64:
            parse_cmd_seg64(cmd, c, stable);
            break;
        case CMD_SYMTABLE:
            parse_cmd_symtable(cmd, c, stable);
            break;
        default:
            break;
    }

}

void parse_head(header_macho m) {
    if (opts.no_head == OFF && opts.silence == OFF) {
        if (opts.head == ON) {
            print_header(m);
            exit(0);
        }
        print_header(m);
        printf("\n----Load Commands----\n");
    }
}

