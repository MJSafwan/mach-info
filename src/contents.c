#include <string.h>

#include "contents.h"

size_t contents_read(void *buf, size_t size, size_t num, read_type type, contents_t *contents) {
    
    while (num != 0) {
        if (contents->offset + num * size > contents->size) {
            num--;
        }
        break;
    }
    if (num == 0) return 0;

    memcpy(buf, &contents->items[contents->offset], size * num);
    if (type == SEEK) {
        contents->offset += size * num;
    }
    /* todo: error handling */
    return num;
}

size_t contents_seek(size_t offset, contents_t *contents) {
    contents->offset += offset;
    return offset;
}

