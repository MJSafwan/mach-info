#include "contents.h"
#include "macho.h"
#include "sectable.h"

#ifndef PARSE_H_
#define PARSE_H_

void parse_cmd(loadcmd cmd, size_t i, contents_t *c, section_table *stable);
void parse_head(header_macho m);

#endif
