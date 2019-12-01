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

TREE_NODE* create_integer_node(const int *value_ptr);
TREE_NODE* create_float_node(const float *value_ptr);
TREE_NODE* create_symbol_node(char *id);
TREE_NODE* create_assignment_node(TREE_NODE *symbol, TREE_NODE *value);
TREE_NODE* create_sum_node(TREE_NODE *left_value, TREE_NODE *right_value);
TREE_NODE* create_minus_node(TREE_NODE *left_value, TREE_NODE *right_value);
TREE_NODE* create_times_node(TREE_NODE *left_value, TREE_NODE *right_value);
TREE_NODE* create_divide_node(TREE_NODE *left_value, TREE_NODE *right_value);
TREE_NODE* create_pow_node(TREE_NODE *left_value, TREE_NODE *right_value);

void add_print_node(TREE_NODE *expression_node);
void add_assignment_node(char *symbol, TREE_NODE *value);

#endif //CALCULATOR_LAGUAGE_SEMANTIC_TREE_H
