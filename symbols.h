#ifndef CALCULATORLANGUAGE_SYMBOLS_H
#define CALCULATORLANGUAGE_SYMBOLS_H

#include "list.h"

typedef struct {
    char *id;
    float value;
} SYMBOL;

SYMBOL* get_symbol(char *symbol);
void update_symbol_value(char *symbol, float value);
SYMBOL *alloc_new_symbol(char *id);
int is_allocated(char *symbol);

#endif //CALCULATORLANGUAGE_SYMBOLS_H
