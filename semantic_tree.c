#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "semantic_tree.h"
#include "binary_tree.h"
#include "symbols.h"

void initialize_semantic_tree() {
    semantic_trees_list = LIST_new();
}

TREE_NODE *create_float_node(const float *value_ptr) {
    float *value = (float*) malloc(sizeof(float));
    *value = *value_ptr;
    return BINARY_TREE__create_node(FLOAT_NODE, value);
}

TREE_NODE *create_symbol_node(char *id) {
    return BINARY_TREE__create_node(SYMBOL_NODE, id);
}

TREE_NODE *create_sum_node(TREE_NODE *left_value, TREE_NODE *right_value) {
    return BINARY_TREE__create_binary_node(SUM_OPERATION_NODE, left_value, right_value);
}

TREE_NODE *create_minus_node(TREE_NODE *left_value, TREE_NODE *right_value) {
    return BINARY_TREE__create_binary_node(MINUS_OPERATION_NODE, left_value, right_value);
}

TREE_NODE *create_times_node(TREE_NODE *left_value, TREE_NODE *right_value) {
    return BINARY_TREE__create_binary_node(TIMES_OPERATION_NODE, left_value, right_value);
}

TREE_NODE *create_divide_node(TREE_NODE *left_value, TREE_NODE *right_value) {
    return BINARY_TREE__create_binary_node(DIVIDE_OPERATION_NODE, left_value, right_value);
}

TREE_NODE *create_pow_node(TREE_NODE *left_value, TREE_NODE *right_value) {
    return BINARY_TREE__create_binary_node(POWER_OPERATION_NODE, left_value, right_value);
}

void add_print_node(TREE_NODE *expression_node) {
    LIST_append(semantic_trees_list, BINARY_TREE__create_binary_node(PRINT_NODE, NULL, expression_node));
}

void add_assignment_node(char *symbol, TREE_NODE *value) {
    TREE_NODE *assignment_node = BINARY_TREE__create_binary_node(ASSIGNMENT_NODE, NULL, value);
    assignment_node->content = symbol;

    LIST_append(semantic_trees_list, assignment_node);
}

void *evaluate(const TREE_NODE *tree_node) {
    switch (tree_node->type_flag) {
        case FLOAT_NODE:
            return tree_node->content;
        case SYMBOL_NODE: {
            SYMBOL *symbol = get_symbol(tree_node->content);

            return &(symbol->value);
        }
        case ASSIGNMENT_NODE: {
            float value = *((float *) evaluate(tree_node->right));

            update_symbol_value(tree_node->content, value);

            return create_symbol_node(tree_node->content);
        }

        case SUM_OPERATION_NODE: {
            float left_value = *((float *) evaluate(tree_node->left));
            float right_value = *((float *) evaluate(tree_node->right));

            float *result_ptr = malloc(sizeof(float));
            *result_ptr = left_value + right_value;

            return result_ptr;
        }

        case MINUS_OPERATION_NODE: {
            float left_value = *((float *) evaluate(tree_node->left));
            float right_value = *((float *) evaluate(tree_node->right));

            float *result_ptr = malloc(sizeof(float));
            *result_ptr = left_value - right_value;

            return result_ptr;
        }
        case TIMES_OPERATION_NODE: {
            float left_value = *((float *) evaluate(tree_node->left));
            float right_value = *((float *) evaluate(tree_node->right));

            float *result_ptr = malloc(sizeof(float));
            *result_ptr = left_value * right_value;

            return result_ptr;
        }
        case DIVIDE_OPERATION_NODE: {
            float left_value = *((float *) evaluate(tree_node->left));
            float right_value = *((float *) evaluate(tree_node->right));

            float *result_ptr = malloc(sizeof(float));
            *result_ptr = left_value / right_value;

            return result_ptr;
        }
        case POWER_OPERATION_NODE: {
            float left_value = *((float *) evaluate(tree_node->left));
            float right_value = *((float *) evaluate(tree_node->right));

            float *result_ptr = malloc(sizeof(float));
            *result_ptr = powf(left_value, right_value);

            return result_ptr;
        }
        case PRINT_NODE: {
            float right_value = *((float *) evaluate(tree_node->right));

            printf("%f\n", right_value);
        }
    }
}

void execute_tree(int index) {
    LIST_ELEMENT *list_element = semantic_trees_list->head;

    while (index--) {
        list_element = list_element->next;
    }
    const TREE_NODE *tree = list_element->content;
    evaluate(tree);
}

void execute_all() {
    LIST_ELEMENT *list_element = semantic_trees_list->head;

    while (list_element != NULL) {
        evaluate(list_element->content);
        list_element = list_element->next;
    }
}
