#include "macho.h"

#ifndef HANDLER_H_
#define HANDLER_H_

#define READ_ERR_MSG "An error happened while reading the file!"

void fetal(char* msg);
void handle_hstatus(header_status status);
void handle_loadcmd(loadcmd_status status);

#endif
