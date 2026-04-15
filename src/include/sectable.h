#include <stdlib.h>
#include <stdio.h>

#ifndef _SECTABLE_H
#define _SECTABLE_H

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} section_table;

void section_table_push(section_table *table, char* entry);

void section_table_free(section_table *table);

#endif
