#include <stdio.h>
#include <malloc.h>
#include "symbols.h"
#include "string.h"
#include "list.h"

LIST *symbols;

extern void initialize_symbol_table() {
    symbols = (LIST*) malloc(sizeof(LIST));

    symbols->head = NULL;
    symbols->tail = NULL;
}

SYMBOL *alloc_new_symbol(char *id, float value) {
    SYMBOL *new_symbol = (SYMBOL *) malloc(sizeof(SYMBOL));

    new_symbol->id = id;
    new_symbol->value = value;

    return new_symbol;
}

int search_symbol_by_id(SYMBOL *symbol, char *id) {
    return strcmp(symbol->id, id) == 0;
}

int search_proxy(void* symbol, void* id) {
    return search_symbol_by_id(symbol, id);
}

extern void update_symbol_value(char *symbol, float value) {

    SYMBOL *current = LIST_search(symbols, &search_proxy, symbol);

    if(current != NULL) {
        current->value = value;
    } else {
        LIST_append(symbols, alloc_new_symbol(symbol, value));
    }
}

extern float get_symbol_value(char *symbol) {

    SYMBOL *current = LIST_search(symbols, &search_proxy, symbol);

    if (current == NULL) {
        printf("Symbol %s not previously defined", symbol);
        return 0.0f;
    }

    return current->value;
}