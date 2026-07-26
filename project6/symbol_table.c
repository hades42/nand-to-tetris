#include "symbol_table.h"
#include <string.h>

void symbol_table_init(symbol_table *table) {
    table->count = 0;
}

int symbol_table_add(symbol_table *table, const char *symbol, int address) {
    if (symbol_table_contain(table, symbol)) {
        return 0;
    }

    if (strlen(symbol) >= MAX_SYMBOL_LENGTH) {
        return 0;
    }

    if (table->count >= MAX_SYMBOLS) {
        return 0;
    }

    symbol_entry *new_symbol = &table->entries[table->count];
    new_symbol->address = address;
    strcpy(new_symbol->name, symbol);
    table->count++;

    return 1;
}

int symbol_table_contain(symbol_table *table, const char *symbol) {
    for (size_t step = 0; step < table->count; step += 1) {
        if(strcmp(table->entries[step].name, symbol) == 0) {
            return 1;
        }
    }
    return 0;
}

int symbol_table_get_address(symbol_table *table, const char *symbol) {
    for (size_t step = 0; step < table->count; step += 1) {
       if(strcmp(table->entries[step].name, symbol) == 0) {
           return table->entries[step].address;
       }
    }
    return -1;
}
