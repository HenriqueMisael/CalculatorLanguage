#ifndef CALCULATOR_LAGUAGE_SEMANTIC_TREE_H
#define CALCULATOR_LAGUAGE_SEMANTIC_TREE_H

#include "list.h"
#include "binary_tree.h"

#define FLOAT_NODE 1
#define SYMBOL_NODE 2
#define ASSIGNMENT_NODE 3
#define SUM_OPERATION_NODE 4
#define MINUS_OPERATION_NODE 5
#define TIMES_OPERATION_NODE 6
#define DIVIDE_OPERATION_NODE 7
#define POWER_OPERATION_NODE 8
#define PRINT_NODE 9
#define INT_NODE 10

LIST *semantic_trees_list;

TREE_NODE* create_integer_node(int line, int column, const int *value_ptr);
TREE_NODE* create_float_node(int line, int column, const float *value_ptr);
TREE_NODE* create_symbol_node(int line, int column, char *id);
TREE_NODE* create_sum_node(int line, int column, TREE_NODE *left_value, TREE_NODE *right_value);
TREE_NODE* create_minus_node(int line, int column, TREE_NODE *left_value, TREE_NODE *right_value);
TREE_NODE* create_times_node(int line, int column, TREE_NODE *left_value, TREE_NODE *right_value);
TREE_NODE* create_divide_node(int line, int column, TREE_NODE *left_value, TREE_NODE *right_value);
TREE_NODE* create_pow_node(int line, int column, TREE_NODE *left_value, TREE_NODE *right_value);

void add_print_node(int line, int column, TREE_NODE *expression_node);
void add_assignment_node(int line, int column, char *symbol, TREE_NODE *value);
void print_error(int code, int line, int column);

#define ERROR_UNUSED_EXPRESSION 1
#define ERROR_OP_NOT_ALLOWED_ASSIGN_NON_NUMBER 2
#define ERROR_OP_NOT_ALLOWED_ASSIGN_OTHER_TYPE 3
#define ERROR_OP_NOT_ALLOWED_SUM_OP_BETWEEN_DIFFERENT_TYPES 4
#define ERROR_OP_NOT_ALLOWED_MINUS_OP_BETWEEN_DIFFERENT_TYPES 5

#endif //CALCULATOR_LAGUAGE_SEMANTIC_TREE_H
