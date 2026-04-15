#include "parse.h"

static int can_print(loadcmd cmd) {
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

static void parse_cmd_seg64(loadcmd cmd) {
    uint32_t nsec = cmd.as_cmd_seg64.num_of_sec;
    for (size_t k = 0; k < nsec; k++) {
        loadcmd_sec64 sec64 = {0};
        handle_loadcmd(loadcmd_sec_get(&sec64, f), f);
        if (can_print(cmd)) {
            if (opts.no_sec == OFF) {
                printf("\nSection %zu:\n\n", k);
                print_loadcmd_sec(sec64);
            }
        }
        section_table_push(&stable, strdup(sec64.sec_name));
    }
}

static void parse_cmd_symtable(loadcmd cmd) {
    uint32_t nsym = cmd.as_cmd_symtable.number_of_symbols;
    fpos_t pos;

    (void) fgetpos(f, &pos);

    char *string_table = alloca(cmd.as_cmd_symtable.string_size);
    (void) fseek(f, cmd.as_cmd_symtable.string_offset, SEEK_SET);
    if (fread(string_table, cmd.as_cmd_symtable.string_size, 1, f) != 1) {
        (void) fsetpos(f, &pos);
        return;
    }

    (void) fseek(f, cmd.as_cmd_symtable.symbol_offset, SEEK_SET);

    for (size_t k = 0; k < nsym; k++) {
        loadcmd_symentry entry = {0};
        handle_loadcmd(loadcmd_symentry_get(&entry, f), f);
        if (can_print(cmd)) {
            if (opts.no_sym == OFF) {
                printf("\n\nSymbol %zu:\n\n", k);
                print_loadcmd_symbol(entry, string_table, stable);
            }
        }
    }

    (void) fsetpos(f, &pos);

}

void parse_cmd(loadcmd cmd, size_t i) {

    if (can_print(cmd)) {
        printf("\n\nLoad Command %zu:\n\n", i);
        print_loadcmd(cmd);
    }

    switch (cmd.type) {
        case CMD_SEG64:
            parse_cmd_seg64(cmd);
            break;
        case CMD_SYMTABLE:
            parse_cmd_symtable(cmd);
            break;
        default:
            break;
    }

}

void parse_head(header_macho m) {
    if (opts.no_head == OFF) {
        if (opts.head == ON) {
            print_header(m);
            exit(0);
        }
        print_header(m);
        printf("\n----Load Commands----\n");
    }
}

