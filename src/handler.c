#include "handler.h"

void fetal(char* msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);
    exit(1);
}

void handle_hstatus(header_status status) {
    switch (status) {
        case HEADER_ERR_READ:
            fetal(READ_ERR_MSG);
            break;
        case HEADER_ERR_NOT_MACHO64:
            fetal("The file is not a 64-bit Mach-O file!");
            break;
        case HEADER_ERR_FAT:
            fetal("Cannot handle fat Mach-O files yet!");
            break;
        case HEADER_FINE:
        default:
            break;
    }
}


void handle_loadcmd(loadcmd_status status) {
    switch(status) {
        case LOADCMD_ERR_READ:
            fetal(READ_ERR_MSG);
            break;
        case LOADCMD_ERR_UNKNOWN_CMD:
            /* Do nothing for now. */
            break;
        case LOADCMD_FINE:
        default:
            break;
    }
}
