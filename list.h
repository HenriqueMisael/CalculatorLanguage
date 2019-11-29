#ifndef CALCULATOR_LAGUAGE_LIST_H
#define CALCULATOR_LAGUAGE_LIST_H

typedef struct LIST_ELEMENT {
    struct LIST_ELEMENT *next;
    void *content;
} LIST_ELEMENT;

typedef struct {
    LIST_ELEMENT *head;
    LIST_ELEMENT *tail;
} LIST;

void LIST_append(LIST *list, void *new_element);

void *LIST_search(LIST *list, int (*func_ptr)(void *, void *), void *extra_args);

LIST *LIST_new();

#endif //CALCULATOR_LAGUAGE_LIST_H
