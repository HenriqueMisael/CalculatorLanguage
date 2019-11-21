#include <stdio.h>
#include "symbols.h"

SYMBOL_LIST *symbols;

extern void initialize_symbol_table() {
    symbols = (SYMBOL_LIST *) malloc(sizeof(SYMBOL_LIST));

    symbols->head = NULL;
    symbols->tail = NULL;
}

SYMBOL *alloc_new_symbol(char *id, float value) {
    SYMBOL *new_symbol = (SYMBOL *) malloc(sizeof(SYMBOL));

    new_symbol->id = id;
    new_symbol->value = value;
    new_symbol->next = NULL;

    return new_symbol;
}

extern void update_symbol_value(char *symbol, float value) {


    if (symbols->tail == NULL && symbols->head == NULL) {
        SYMBOL *new_symbol_ptr = alloc_new_symbol(symbol, value);
        symbols->head = new_symbol_ptr;
        symbols->tail = new_symbol_ptr;
    } else {
        SYMBOL *current = symbols->head;
        do {
            if (strcmp(current->id, symbol) == 0) {
                current->value = value;
                return;
            }
            current = current->next;
        } while (current != NULL);

        SYMBOL *new_symbol_ptr = alloc_new_symbol(symbol, value);
        symbols->tail->next = new_symbol_ptr;
        symbols->tail = new_symbol_ptr;
    }
}

extern float get_symbol_value(char *symbol) {

    SYMBOL *current = symbols->head;
    do {
        if (strcmp(current->id, symbol) == 0) {
            return current->value;
        }
        current = current->next;
    } while (current != NULL);

    printf("Symbol %s is not allocated yet", symbol);

    return 0.0;
}