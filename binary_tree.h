#ifndef CALCULATOR_LAGUAGE_BINARY_TREE_H
#define CALCULATOR_LAGUAGE_BINARY_TREE_H

typedef struct TREE_NODE {
    struct TREE_NODE *left;
    struct TREE_NODE *right;
    int type_flag;
    void *content;
} TREE_NODE;


TREE_NODE *BINARY_TREE__create_node(int type_flag, void *content);
TREE_NODE *BINARY_TREE__create_binary_node(int type_flag, TREE_NODE *left_node, TREE_NODE *right_node);

#endif //CALCULATOR_LAGUAGE_BINARY_TREE_H
