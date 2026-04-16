#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef _ARGS_H
#define _ARGS_H

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
             "-h or -help                         : Displays this\n"\
             "-head                               : Only shows header\n"\
             "-seg64                              : Only shows segments\n"\
             "-symtab                             : Only shows the symbol table\n"\
             "-no-head                            : Hides the header\n"\
             "-no-sec                             : Hides sections\n"\
             "-no-sym                             : Hides the symbol table entries\n"\
             "-seg64-info <seg64 name>            : Shows information about <seg64 name>\n"\
             "-dump <seg64 name> <sec name> <out> : Outputs the binary data of <sec name> in <seg64 name> to <out> file\n"\

void parse_args(int argc, char** argv);

#endif
