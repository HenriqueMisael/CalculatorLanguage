#include <stdio.h>
#include <malloc.h>
#include "binary_tree.h"

TREE_NODE *BINARY_TREE__create_node(int type_flag, void *content) {
    TREE_NODE *new_node = (TREE_NODE *) malloc(sizeof(TREE_NODE));

    new_node->content = content;
    new_node->type_flag = type_flag;

    return new_node;
}

TREE_NODE *BINARY_TREE__create_binary_node(int type_flag, TREE_NODE *left_node, TREE_NODE *right_node) {
    TREE_NODE *node = BINARY_TREE__create_node(type_flag, NULL);

    node->left = left_node;
    node->right = right_node;

    return node;
}
