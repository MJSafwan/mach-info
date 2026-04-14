#include "print.h"
#define LC_SEGMENT_64 "LC_SEGMENT_64"
#define LC_MAIN "LC_MAIN"
#define LC_SYMTAB "LC_SYMTAB"

void print_header(header_macho m) {
    printf("Magic: 0x%x\n", m.magic);
    printf("CPU type: 0x%x\n", m.cpu_type);
    printf("CPU subtype: 0x%x\n", m.cpu_subtype);
    printf("Filetype: 0x%x\n", m.filetype);
    printf("Number of load commands: %d\n", m.num_of_load_cmds);
    printf("Size of load commands: %d\n", m.size_of_load_cmds);
    printf("Flags: 0x%x\n", m.flags);
    printf("Reserved: 0x%x\n", m.reserved);
}

void print_cmdseg64(loadcmd_seg64 l) {
    printf("Command type = "LC_SEGMENT_64"\n");
    printf("Command size = %u\n", l.size);
    printf("Segment name = %.16s\n", l.name);
    printf("Address = 0x%llx\n", l.address);
    printf("Address size = 0x%llx\n", l.address_size);
    printf("File offset = 0x%llx\n", l.file_offset);
    printf("File size = %llu\n", l.filesize);
    printf("Max memory protection = 0x%x\n", l.max_protect);
    printf("Initial memory protection = 0x%x\n", l.init_protect);
    printf("Number of sections = %u\n", l.num_of_sec);
    printf("Flags32 = 0x%u\n", l.flags32);
}

void print_loadcmd_sec(loadcmd_sec64 sec64) {
    printf("Section name: %.16s\n", sec64.sec_name);
    printf("Segment name: %.16s\n", sec64.seg_name);
    printf("Address: 0x%llx\n", sec64.addr);
    printf("Size: %llu\n", sec64.size);
    printf("Offset: 0x%x\n", sec64.offset);
    printf("Align: %d\n", sec64.align);
    printf("Relocation offset: 0x%x\n", sec64.reloff);
    printf("Number of relocations: %d\n", sec64.nreloc);
    printf("Flags: 0x%x\n", sec64.flags);
    printf("Reserved 1: 0x%x\n", sec64.reserved1);
    printf("Reserved 2: 0x%x\n", sec64.reserved2);
    printf("Reserved 3: 0x%x\n", sec64.reserved3);
}

void print_cmdmain(loadcmd_main l) {
    printf("Command type = "LC_MAIN"\n");
    printf("Command size = %u\n", l.size);
    printf("Offset = 0x%llx\n", l.offset);
    printf("Stack size = %llu\n", l.stack_size);
}

void print_cmdsymtable(loadcmd_symtable l) {
    printf("Command type = "LC_SYMTAB"\n");
    printf("Command size = %u\n", l.size);
    printf("Offset of symbol table = 0x%x\n", l.symbol_offset);
    printf("Number of symbols = %u\n", l.number_of_symbols);
    printf("Offset of string table = 0x%x\n", l.string_offset);
    printf("Size of string table = %u\n", l.string_size);
}

void print_loadcmd_symbol(loadcmd_symentry entry, char* string_table, section_table stable) {
    printf("String: %s (0x%x)\n", &string_table[entry.n_strx], entry.n_strx);
    printf("Symbol's type: 0x%x\n", entry.n_type);
    printf("Symbol's section: %.16s (%d)\n", stable.items[entry.n_sect - 1], entry.n_sect);
    printf("Flags: 0x%x\n", entry.n_desc);
    printf("Symbol's value: 0x%llx", entry.n_value);
}

void print_loadcmd(loadcmd lcmd) {
    switch(lcmd.type) {
        case CMD_SEG64:
            print_cmdseg64(lcmd.as_cmd_seg64);
            break;
        case CMD_MAIN:
            print_cmdmain(lcmd.as_cmd_main);
            break;
        case CMD_SYMTABLE:
            print_cmdsymtable(lcmd.as_cmd_symtable);
            break;
        default:
            break;
    }   
}
