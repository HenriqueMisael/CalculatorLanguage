#include <stdio.h>
#include <malloc.h>
#include "list.h"

void LIST_append(LIST *list, void *new_element_content) {
    LIST_ELEMENT *new_element = (LIST_ELEMENT *) malloc(sizeof(LIST_ELEMENT));
    new_element->content = new_element_content;

    if (list->head == NULL) {
        list->head = new_element;
        list->tail = new_element;
    } else {
        list->tail->next = new_element;
        list->tail = new_element;
    }
}

void *LIST_search(LIST *list, int (*func_ptr)(void *, void *), void *extra_args) {

    LIST_ELEMENT *current = list->head;

    while (current != NULL) {
        if (func_ptr(current->content, extra_args)) {
            return current->content;
        }
        current = current->next;
    }

    return NULL;
}