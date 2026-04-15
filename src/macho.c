#include "macho.h"
#include "sizes.h"

header_status header_get(header_macho* h, contents_t *contents) {
    if(contents_read(h, sizeof(header_macho), 1, SEEK, contents) != 1) {
        return HEADER_ERR_READ;
    }

    if (h->magic == MAGIC64_FAT) return HEADER_ERR_FAT;
    if (h->magic != MAGIC64) return HEADER_ERR_NOT_MACHO64;

    return HEADER_FINE;
}

loadcmd_status loadcmd_get(loadcmd *lcmd, contents_t *contents) {
    if(contents_read(lcmd, sizeof(uint32_t), 2, NO_SEEK, contents) != 2) {
        return LOADCMD_ERR_READ;
    }

    uint32_t curr_cmd_size = 0;
    if(sizes_get(lcmd->type, &curr_cmd_size) == SIZES_NOT_FOUND) { 
        (void) contents_seek(lcmd->size, contents);
        return LOADCMD_ERR_UNKNOWN_CMD;   
    }
    if (contents_read(&lcmd->as_cmd, curr_cmd_size, 1, SEEK, contents) != 1) return LOADCMD_ERR_READ;

    return LOADCMD_FINE;
}

loadcmd_status loadcmd_sec_get(loadcmd_sec64 *sec, contents_t *contents) {
    if (contents_read(sec, sizeof(loadcmd_sec64), 1, SEEK, contents) != 1) {
        return LOADCMD_ERR_READ;
    }

    return LOADCMD_FINE;
}

loadcmd_status loadcmd_symentry_get(loadcmd_symentry *entry, contents_t *contents) {
    if (contents_read(entry, sizeof(loadcmd_symentry), 1, SEEK, contents) != 1) {
        return LOADCMD_ERR_READ;
    }

    return LOADCMD_FINE;
}

