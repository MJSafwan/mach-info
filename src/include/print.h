#include <stdio.h>
#include "macho.h"
#include "utils.h"

#ifndef _PRINT_H
#define _PRINT_H

void print_header(header_macho m);

void print_cmdseg64(loadcmd_seg64 seg64);
void print_cmdmain(loadcmd_main m);
void print_cmdsymtable(loadcmd_symtable s);

void print_loadcmd(loadcmd lcmd);
void print_loadcmd_sec(loadcmd_sec64 sec64);
void print_loadcmd_symbol(loadcmd_symentry entry, char* string_table, section_table stable);

#endif
