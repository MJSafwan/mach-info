
#ifndef CONTENTS_H_
#define CONTENTS_H_

typedef struct {
    char *items;
    size_t size;
    int offset;
} contents_t;

typedef enum {
    NO_SEEK,
    SEEK,
} read_type;

size_t contents_read(void *buf, size_t size, size_t num, read_type type, contents_t *contents);

size_t contents_seek(size_t offset, contents_t *contents);

#endif
