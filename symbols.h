#ifndef CALCULATORLANGUAGE_SYMBOLS_H
#define CALCULATORLANGUAGE_SYMBOLS_H

#include "list.h"

typedef union {float floating; int integer;} VALUE;

typedef struct {
    char *id;
    int value_type;
    VALUE *value;
} SYMBOL;

SYMBOL* get_symbol(char *symbol);
void update_symbol_value(char *symbol, VALUE *value, int value_type);
SYMBOL *alloc_new_symbol(char *id, int value_type);
int is_allocated(char *symbol);

#endif //CALCULATORLANGUAGE_SYMBOLS_H
