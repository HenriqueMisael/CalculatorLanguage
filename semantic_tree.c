#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include "semantic_tree.h"
#include "binary_tree.h"
#include "symbols.h"

void initialize_semantic_tree() {
    semantic_trees_list = LIST_new();
}

TREE_NODE *create_integer_node(int line, int column, const int *value_ptr) {
    int *value = (int *) malloc(sizeof(int));
    *value = *value_ptr;
    return BINARY_TREE__create_node(INT_NODE, value, line, column);
}

TREE_NODE *create_float_node(int line, int column, const float *value_ptr) {
    float *value = (float *) malloc(sizeof(float));
    *value = *value_ptr;
    return BINARY_TREE__create_node(FLOAT_NODE, value, line, column);
}

TREE_NODE *create_symbol_node(int line, int column, char *id) {
    return BINARY_TREE__create_node(SYMBOL_NODE, id, line, column);
}

TREE_NODE *create_sum_node(int line, int column, TREE_NODE *left_value, TREE_NODE *right_value) {
    return BINARY_TREE__create_binary_node(SUM_OPERATION_NODE, left_value, right_value, line, column);
}

TREE_NODE *create_minus_node(int line, int column, TREE_NODE *left_value, TREE_NODE *right_value) {
    return BINARY_TREE__create_binary_node(MINUS_OPERATION_NODE, left_value, right_value, line, column);
}

TREE_NODE *create_times_node(int line, int column, TREE_NODE *left_value, TREE_NODE *right_value) {
    return BINARY_TREE__create_binary_node(TIMES_OPERATION_NODE, left_value, right_value, line, column);
}

TREE_NODE *create_divide_node(int line, int column, TREE_NODE *left_value, TREE_NODE *right_value) {
    return BINARY_TREE__create_binary_node(DIVIDE_OPERATION_NODE, left_value, right_value, line, column);
}

TREE_NODE *create_pow_node(int line, int column, TREE_NODE *left_value, TREE_NODE *right_value) {
    return BINARY_TREE__create_binary_node(POWER_OPERATION_NODE, left_value, right_value, line, column);
}

void add_print_node(int line, int column, TREE_NODE *expression_node) {
    LIST_append(semantic_trees_list, BINARY_TREE__create_binary_node(PRINT_NODE, NULL, expression_node, line, column));
}

void add_assignment_node(int line, int column, char *symbol, TREE_NODE *value) {
    TREE_NODE *assignment_node = BINARY_TREE__create_binary_node(ASSIGNMENT_NODE, NULL, value, line, column);
    assignment_node->content = symbol;

    LIST_append(semantic_trees_list, assignment_node);
}

int check_value_type(TREE_NODE *tree_node) {
    switch (tree_node->type_flag) {
        case INT_NODE:
        case FLOAT_NODE:
            return tree_node->type_flag;
        case SYMBOL_NODE:
            return get_symbol((char *) tree_node->content)->value_type;
        case SUM_OPERATION_NODE:
        case MINUS_OPERATION_NODE:
        case POWER_OPERATION_NODE:
            return check_value_type(tree_node->left);
        case DIVIDE_OPERATION_NODE:
        case TIMES_OPERATION_NODE:
            if (check_value_type(tree_node->right) == FLOAT_NODE) return FLOAT_NODE;
            if (check_value_type(tree_node->left) == FLOAT_NODE) return FLOAT_NODE;
            return INT_NODE;
        default:
            return -1;
    }
}

void *evaluate(const TREE_NODE *tree_node) {
    switch (tree_node->type_flag) {
        case FLOAT_NODE:
        case INT_NODE:
            return tree_node->content;
        case SYMBOL_NODE: {
            SYMBOL *symbol = get_symbol(tree_node->content);
            if (symbol->value_type == INT_NODE) {
                return &symbol->value->integer;
            }
            return &symbol->value->floating;
        }
        case ASSIGNMENT_NODE: {

            VALUE *value = malloc(sizeof(VALUE));
            int value_type = check_value_type(tree_node->right);
            if (value_type == FLOAT_NODE)
                value->floating = *((float *) evaluate(tree_node->right));
            else if (value_type == INT_NODE) value->integer = *((int *) evaluate(tree_node->right));
            else print_error(ERROR_OP_NOT_ALLOWED_ASSIGN_NON_NUMBER, tree_node->line, tree_node->column);

            if (is_allocated(tree_node->content) && get_symbol(tree_node->content)->value_type != value_type) {
                print_error(
                        ERROR_OP_NOT_ALLOWED_ASSIGN_OTHER_TYPE, tree_node->line, tree_node->column);
            }

            update_symbol_value(tree_node->content, value, value_type);

            return create_symbol_node(tree_node->line, tree_node->column, tree_node->content);
        }

        case SUM_OPERATION_NODE: {
            int left_value_type = check_value_type(tree_node->left);
            int right_value_type = check_value_type(tree_node->right);
            if (left_value_type != right_value_type || (left_value_type != INT_NODE && left_value_type != FLOAT_NODE)) {
                print_error(ERROR_OP_NOT_ALLOWED_SUM_OP_BETWEEN_DIFFERENT_TYPES, tree_node->line, tree_node->column);
            }

            if (left_value_type == FLOAT_NODE) {
                float left_value = *((float *) evaluate(tree_node->left));
                float right_value = *((float *) evaluate(tree_node->right));

                float *result_ptr = malloc(sizeof(float));
                *result_ptr = left_value + right_value;

                return result_ptr;
            } else {
                int left_value = *((int *) evaluate(tree_node->left));
                int right_value = *((int *) evaluate(tree_node->right));

                int *result_ptr = malloc(sizeof(int));
                *result_ptr = left_value + right_value;

                return result_ptr;
            }
        }
        case MINUS_OPERATION_NODE: {
            int left_value_type = check_value_type(tree_node->left);
            int right_value_type = check_value_type(tree_node->right);
            if (left_value_type != right_value_type || (left_value_type != INT_NODE && left_value_type != FLOAT_NODE)) {
                print_error(ERROR_OP_NOT_ALLOWED_MINUS_OP_BETWEEN_DIFFERENT_TYPES, tree_node->line, tree_node->column);
            }

            if (left_value_type == FLOAT_NODE) {
                float left_value = *((float *) evaluate(tree_node->left));
                float right_value = *((float *) evaluate(tree_node->right));

                float *result_ptr = malloc(sizeof(float));
                *result_ptr = left_value - right_value;

                return result_ptr;
            } else {
                int left_value = *((int *) evaluate(tree_node->left));
                int right_value = *((int *) evaluate(tree_node->right));

                int *result_ptr = malloc(sizeof(int));
                *result_ptr = left_value - right_value;

                return result_ptr;
            }
        }
        case TIMES_OPERATION_NODE: {
            int left_value_type = check_value_type(tree_node->left);
            int right_value_type = check_value_type(tree_node->right);

            float *left_value_ptr = NULL;
            float *right_value_ptr = NULL;
            float *result_ptr = malloc(sizeof(float));

            if (left_value_type == INT_NODE) {
                int value = *(int *) evaluate(tree_node->left);
                left_value_ptr = (float *) malloc(sizeof(float));

                *left_value_ptr = value;

            } else if (left_value_type == FLOAT_NODE) {
                left_value_ptr = (float *) evaluate(tree_node->left);
            }
            if (right_value_type == INT_NODE) {
                int value = *(int *) evaluate(tree_node->right);
                right_value_ptr = (float *) malloc(sizeof(float));

                *right_value_ptr = value;

            } else if (right_value_type == FLOAT_NODE) {
                right_value_ptr = (float *) evaluate(tree_node->right);
            }

            *result_ptr = *left_value_ptr * *right_value_ptr;

            if (left_value_type == FLOAT_NODE || right_value_type == FLOAT_NODE) {
                return result_ptr;
            }
            int *integer_result_ptr = (int *) malloc(sizeof(int));
            *integer_result_ptr = *result_ptr;
            return integer_result_ptr;
        }
        case DIVIDE_OPERATION_NODE: {
            int left_value_type = check_value_type(tree_node->left);
            int right_value_type = check_value_type(tree_node->right);

            float *left_value_ptr = NULL;
            float *right_value_ptr = NULL;
            float *result_ptr = malloc(sizeof(float));

            if (left_value_type == INT_NODE) {
                int value = *(int *) evaluate(tree_node->left);
                left_value_ptr = (float *) malloc(sizeof(float));

                *left_value_ptr = value;

            } else if (left_value_type == FLOAT_NODE) {
                left_value_ptr = (float *) evaluate(tree_node->left);
            }
            if (right_value_type == INT_NODE) {
                int value = *(int *) evaluate(tree_node->right);
                right_value_ptr = (float *) malloc(sizeof(float));

                *right_value_ptr = value;

            } else if (right_value_type == FLOAT_NODE) {
                right_value_ptr = (float *) evaluate(tree_node->right);
            }

            *result_ptr = *left_value_ptr / *right_value_ptr;

            if (left_value_ptr == FLOAT_NODE || right_value_ptr == FLOAT_NODE) {
                return result_ptr;
            }
            int *integer_result_ptr = (int *) malloc(sizeof(int));
            *integer_result_ptr = *result_ptr;
            return integer_result_ptr;
        }
        case POWER_OPERATION_NODE: {
            int left_value_type = check_value_type(tree_node->left);
            int right_value_type = check_value_type(tree_node->right);

            float *left_value_ptr = NULL;
            float *right_value_ptr = NULL;
            float *result_ptr = malloc(sizeof(float));

            if (left_value_type == INT_NODE) {
                int value = *(int *) evaluate(tree_node->left);
                left_value_ptr = (float *) malloc(sizeof(float));

                *left_value_ptr = value;

            } else if (left_value_type == FLOAT_NODE) {
                left_value_ptr = (float *) evaluate(tree_node->left);
            }
            if (right_value_type == INT_NODE) {
                int value = *(int *) evaluate(tree_node->right);
                right_value_ptr = (float *) malloc(sizeof(float));

                *right_value_ptr = value;

            } else if (right_value_type == FLOAT_NODE) {
                right_value_ptr = (float *) evaluate(tree_node->right);
            }

            *result_ptr = powf(*left_value_ptr, *right_value_ptr);

            if (left_value_type == FLOAT_NODE || right_value_type == FLOAT_NODE) {
                return result_ptr;
            }
            int *integer_result_ptr = (int *) malloc(sizeof(int));
            *integer_result_ptr = *result_ptr;
            return integer_result_ptr;
        }
        case PRINT_NODE: {
            switch (check_value_type(tree_node->right)) {
                case INT_NODE: {
                    int right_value = *((int *) evaluate(tree_node->right));
                    printf("%d\n", right_value);
                    break;
                }
                case FLOAT_NODE: {
                    float right_value = *((float *) evaluate(tree_node->right));
                    printf("%f\n", right_value);
                    break;
                }
                default:
                    printf("void");
            }
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

void print_error(int code, int line, int column) {
    char error[256];
    switch (code) {
        case ERROR_UNUSED_EXPRESSION:
            strcpy(error, "Unused expression");
            break;
        case ERROR_OP_NOT_ALLOWED_ASSIGN_NON_NUMBER:
            strcpy(error, "Operation not allowed: to assign a non number value to a variable");
            break;
        case ERROR_OP_NOT_ALLOWED_ASSIGN_OTHER_TYPE:
            strcpy(error, "Operation not allowed: to assign a new value to a variable allocated to another value type");
            break;
        case ERROR_OP_NOT_ALLOWED_SUM_OP_BETWEEN_DIFFERENT_TYPES:
            strcpy(error, "Operation not allowed: to make a sum operation between different types");
            break;
        case ERROR_OP_NOT_ALLOWED_MINUS_OP_BETWEEN_DIFFERENT_TYPES:
            strcpy(error, "Operation not allowed: to make a minus operation between different types");
            break;
        default:
            strcpy(error, "Unknown error");
    }
    printf("(%d:%d) %s", line, column, error);
}