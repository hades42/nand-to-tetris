#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>

#define MAX_SYMBOLS 1024
#define MAX_SYMBOL_LENGTH 64

typedef struct {
    char name[MAX_SYMBOL_LENGTH];
    int address;
} symbol_entry;

typedef struct {
    symbol_entry entries[MAX_SYMBOLS];
    size_t count;
} symbol_table;

void symbol_table_init(symbol_table *table);

int symbol_table_add(
    symbol_table *table,
    const char *symbol,
    int address
);

int symbol_table_contain(
    symbol_table *table,
    const char *symbol
);

int symbol_table_get_address(
    symbol_table *table,
    const char *symbol
);

#endif
