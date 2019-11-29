#include <stdio.h>
#include <malloc.h>
#include "symbols.h"
#include "string.h"
#include "list.h"

LIST *symbols;

void initialize_symbol_table() {
    symbols = LIST_new();
}

SYMBOL *alloc_new_symbol(char *id) {
    SYMBOL *new_symbol = (SYMBOL *) malloc(sizeof(SYMBOL));

    new_symbol->id = id;

    return new_symbol;
}

int search_symbol_by_id(SYMBOL *symbol, char *id) {
    return strcmp(symbol->id, id) == 0;
}

int search_proxy(void *symbol, void *id) {
    return search_symbol_by_id(symbol, id);
}

void update_symbol_value(char *symbol, float value) {

    SYMBOL *current = LIST_search(symbols, &search_proxy, symbol);

    if (current == NULL) {
        current = alloc_new_symbol(symbol);
        LIST_append(symbols, current);
    }

    current->value = value;
}

int is_allocated(char *symbol) {
    return LIST_search(symbols, &search_proxy, symbol) != NULL;
}

SYMBOL *get_symbol(char *symbol) {

    SYMBOL *current = LIST_search(symbols, &search_proxy, symbol);

    if (current == NULL) {
        printf("Symbol %s not previously defined", symbol);
        return NULL;
    }

    return current;
}