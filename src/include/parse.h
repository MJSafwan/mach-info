#include "macho.h"
#include "globals.h"
#include "print.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _PARSE_H
#define _PARSE_H

void parse_cmd(loadcmd cmd, size_t i);
void parse_head(header_macho m);

#endif
