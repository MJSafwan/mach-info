
#ifndef SECTABLE_H_
#define SECTABLE_H_

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} section_table;

void section_table_push(section_table *table, char* entry);

void section_table_free(section_table *table);

#endif
