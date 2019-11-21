//
// Created by henri on 19/11/2019.
//

#ifndef CALCULATORLANGUAGE_SYMBOLS_H
#define CALCULATORLANGUAGE_SYMBOLS_H

typedef struct {
    char *id;
    float value;
    struct SYMBOL *next;
} SYMBOL;

typedef struct {
    SYMBOL *head;
    SYMBOL *tail;
} SYMBOL_LIST;

float get_symbol_value(char *symbol);
void update_symbol_value(char *symbol, float value);
SYMBOL *alloc_new_symbol(char *id, float value);

#endif //CALCULATORLANGUAGE_SYMBOLS_H
