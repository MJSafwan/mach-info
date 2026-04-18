
#include "macho.h"
#include "sectable.h"

#ifndef PRINT_H_
#define PRINT_H_

#define LC_SEGMENT_64 "LC_SEGMENT_64"
#define LC_MAIN "LC_MAIN"
#define LC_SYMTAB "LC_SYMTAB"

void print_header(header_macho m);

void print_loadcmd(loadcmd lcmd);
void print_loadcmd_sec(loadcmd_sec64 sec64);
void print_loadcmd_symbol(loadcmd_symentry entry, contents_t *string_table, section_table stable);

#endif
