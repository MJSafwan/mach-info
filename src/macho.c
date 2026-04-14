#include "macho.h"
#include "utils.h"

header_status header_get(header_macho* h, FILE* f) {
    if(fread(h, sizeof(header_macho), 1, f) != 1) {
        return HEADER_ERR_READ;
    }

    if (h->magic == MAGIC64_FAT) return HEADER_ERR_FAT;
    if (h->magic != MAGIC64) return HEADER_ERR_NOT_MACHO64;

    return HEADER_FINE;
}

loadcmd_status loadcmd_get(loadcmd *lcmd, FILE* f) {
    if(fread(lcmd, sizeof(uint32_t), 2, f) != 2) {
        return LOADCMD_ERR_READ;
    }

    (void)fseek(f, -2 * sizeof(uint32_t), SEEK_CUR);
    
    uint32_t curr_cmd_size = 0;
    if(sizes_get(lcmd->type, &curr_cmd_size) == SIZES_NOT_FOUND) { 
        fseek(f, lcmd->size, SEEK_CUR);
        return LOADCMD_ERR_UNKNOWN_CMD;   
    }
    if (fread(&lcmd->as_cmd, curr_cmd_size, 1, f) != 1) return LOADCMD_ERR_READ;
   //(void)fseek(f, lcmd->size-sizeof(loadcmd_seg64), SEEK_CUR);

    return LOADCMD_FINE;
}

loadcmd_sec_status loadcmd_sec_get(loadcmd_sec64 *sec, FILE* f) {
    if (fread(sec, sizeof(loadcmd_sec64), 1, f) != 1) {
        return LOADCMD_SEC_ERR_READ;
    }

    return LOADCMD_SEC_FINE;
}

loadcmd_sym_status loadcmd_symentry_get(loadcmd_symentry *entry, FILE* f) {
    if (fread(entry, sizeof(loadcmd_symentry), 1, f) != 1) {
        return LOADCMD_SYM_ERR_READ;
    }

    return LOADCMD_SYM_FINE;
}

