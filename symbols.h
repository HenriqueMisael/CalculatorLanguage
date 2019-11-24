#ifndef CALCULATORLANGUAGE_SYMBOLS_H
#define CALCULATORLANGUAGE_SYMBOLS_H

#include "list.h"

typedef struct {
    char *id;
    float value;
} SYMBOL;

float get_symbol_value(char *symbol);
void update_symbol_value(char *symbol, float value);
SYMBOL *alloc_new_symbol(char *id, float value);

#endif //CALCULATORLANGUAGE_SYMBOLS_H
