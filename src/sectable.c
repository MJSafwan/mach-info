#include "sectable.h"

void section_table_push(section_table *table, char* entry) {
    if (table->count == table->capacity) {
        table->capacity = table->capacity == 0 ? 1 : table->capacity * 2;
        table->items = realloc(table->items, table->capacity * sizeof(entry));
        if (table->items == NULL) {
            perror("realloc");
            exit(1);
        }
    }
    
    table->items[table->count++] = entry;
}

void section_table_free(section_table *table) {
    for (size_t i = 0; i < table->count; ++i) {
        free(table->items[i]);
    }
    free(table->items);
    table->items = NULL;
}

