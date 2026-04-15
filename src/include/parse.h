#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contents.h"
#include "macho.h"
#include "sectable.h"

#ifndef _PARSE_H
#define _PARSE_H

void parse_cmd(loadcmd cmd, size_t i, contents_t *c, section_table *stable);
void parse_head(header_macho m);

#endif
