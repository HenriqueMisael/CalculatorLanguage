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

TREE_NODE *create_integer_node(const int *value_ptr) {
    int *value = (int *) malloc(sizeof(int));
    *value = *value_ptr;
    return BINARY_TREE__create_node(INT_NODE, value);
}

TREE_NODE *create_float_node(const float *value_ptr) {
    float *value = (float *) malloc(sizeof(float));
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

void print_error(char *error) {
    printf("%s\n", error);
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
            else print_error("Operation not allowed: to assign a non number value to a variable");

            if (is_allocated(tree_node->content) && get_symbol(tree_node->content)->value_type != value_type) {
                print_error(
                        "Operation not allowed: to assign a new value to a variable allocated to another value type");
            }

            update_symbol_value(tree_node->content, value, value_type);

            return create_symbol_node(tree_node->content);
        }

        case SUM_OPERATION_NODE: {
            int left_value_type = check_value_type(tree_node->left);
            int right_value_type = check_value_type(tree_node->right);
            if (left_value_type != right_value_type || (left_value_type != INT_NODE && left_value_type != FLOAT_NODE)) {

                char *error_message = malloc(256);
                sprintf(error_message, "Operation not allowed: to make a sum operation between %s and %s",
                        left_value_type == INT_NODE ? "integer" : left_value_type == FLOAT_NODE ? "float"
                                                                                                : "other",
                        right_value_type == INT_NODE ? "integer" : right_value_type == FLOAT_NODE ? "float"
                                                                                                : "other");
                print_error(error_message);
            }

            if (left_value_type == FLOAT_NODE) {
                float left_value = *((float *) evaluate(tree_node->left));
                float right_value = *((float *) evaluate(tree_node->right));

                float *result_ptr = malloc(sizeof(float));
                *result_ptr = left_value + right_value;

//                printf("%f + %f = %f\n", left_value, right_value, *result_ptr);

                return result_ptr;
            } else {
                int left_value = *((int *) evaluate(tree_node->left));
                int right_value = *((int *) evaluate(tree_node->right));

                int *result_ptr = malloc(sizeof(int));
                *result_ptr = left_value + right_value;

//                printf("%d + %d = %d\n", left_value, right_value, *result_ptr);

                return result_ptr;
            }
        }
        case MINUS_OPERATION_NODE: {
            int left_value_type = check_value_type(tree_node->left);
            int right_value_type = check_value_type(tree_node->right);
            if (left_value_type != right_value_type || (left_value_type != INT_NODE && left_value_type != FLOAT_NODE)) {

                char *error_message = malloc(256);
                sprintf(error_message, "Operation not allowed: to make a minus operation between %s and %s",
                        left_value_type == INT_NODE ? "integer" : left_value_type == FLOAT_NODE ? "float"
                                                                                                : "other",
                        left_value_type == INT_NODE ? "integer" : left_value_type == FLOAT_NODE ? "float"
                                                                                                : "other");
                print_error(error_message);
            }

            if (left_value_type == FLOAT_NODE) {
                float left_value = *((float *) evaluate(tree_node->left));
                float right_value = *((float *) evaluate(tree_node->right));

                float *result_ptr = malloc(sizeof(float));
                *result_ptr = left_value - right_value;

//                printf("%f - %f = %f\n", left_value, right_value, *result_ptr);

                return result_ptr;
            } else {
                int left_value = *((int *) evaluate(tree_node->left));
                int right_value = *((int *) evaluate(tree_node->right));

                int *result_ptr = malloc(sizeof(int));
                *result_ptr = left_value - right_value;

//                printf("%d - %d = %d\n", left_value, right_value, *result_ptr);

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

//                printf("%d", value);
            } else if (left_value_type == FLOAT_NODE) {
                left_value_ptr = (float *) evaluate(tree_node->left);
//                printf("%f", *(float *) left_value_ptr);
            }
//            printf(" * ");
            if (right_value_type == INT_NODE) {
                int value = *(int *) evaluate(tree_node->right);
                right_value_ptr = (float *) malloc(sizeof(float));

                *right_value_ptr = value;

//                printf("%d", value);
            } else if (right_value_type == FLOAT_NODE) {
                right_value_ptr = (float *) evaluate(tree_node->right);
//                printf("%f", *(float *) right_value_ptr);
            }

            *result_ptr = *left_value_ptr * *right_value_ptr;

            if (left_value_ptr == FLOAT_NODE || right_value_ptr == FLOAT_NODE) {
//                printf(" = %f\n", *result_ptr);
                return result_ptr;
            }
            int *integer_result_ptr = (int *) malloc(sizeof(int));
            *integer_result_ptr = *result_ptr;
//            printf(" = %d\n", *integer_result_ptr);
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

//                printf("%d", value);
            } else if (left_value_type == FLOAT_NODE) {
                left_value_ptr = (float *) evaluate(tree_node->left);
//                printf("%f", *(float *) left_value_ptr);
            }
//            printf(" / ");
            if (right_value_type == INT_NODE) {
                int value = *(int *) evaluate(tree_node->right);
                right_value_ptr = (float *) malloc(sizeof(float));

                *right_value_ptr = value;

//                printf("%d", value);
            } else if (right_value_type == FLOAT_NODE) {
                right_value_ptr = (float *) evaluate(tree_node->right);
//                printf("%f", *(float *) right_value_ptr);
            }

            *result_ptr = *left_value_ptr / *right_value_ptr;

            if (left_value_ptr == FLOAT_NODE || right_value_ptr == FLOAT_NODE) {
//                printf(" = %f\n", *result_ptr);
                return result_ptr;
            }
            int *integer_result_ptr = (int *) malloc(sizeof(int));
            *integer_result_ptr = *result_ptr;
//            printf(" = %d\n", *integer_result_ptr);
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

//                printf("%d", value);
            } else if (left_value_type == FLOAT_NODE) {
                left_value_ptr = (float *) evaluate(tree_node->left);
//                printf("%f", *(float *) left_value_ptr);
            }
//            printf(" ^ ");
            if (right_value_type == INT_NODE) {
                int value = *(int *) evaluate(tree_node->right);
                right_value_ptr = (float *) malloc(sizeof(float));

                *right_value_ptr = value;

//                printf("%d", value);
            } else if (right_value_type == FLOAT_NODE) {
                right_value_ptr = (float *) evaluate(tree_node->right);
//                printf("%f", *(float *) right_value_ptr);
            }

            *result_ptr = powf(*left_value_ptr, *right_value_ptr);

            if (left_value_ptr == FLOAT_NODE || right_value_ptr == FLOAT_NODE) {
//                printf(" = %f\n", *result_ptr);
                return result_ptr;
            }
            int *integer_result_ptr = (int *) malloc(sizeof(int));
            *integer_result_ptr = *result_ptr;
//            printf(" = %d\n", *integer_result_ptr);
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
