#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "macho.h"

#ifndef _HANDLER_H
#define _HANDLER_H

#define READ_ERR_MSG "An error happened while reading the file!"

void fetal(char* msg);
void handle_hstatus(header_status status);
void handle_loadcmd(loadcmd_status status);

#endif
