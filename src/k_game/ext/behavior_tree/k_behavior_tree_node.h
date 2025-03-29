#ifndef K_BEHAVIOR_TREE_NODE_H
#define K_BEHAVIOR_TREE_NODE_H

#include "k_list.h"

struct k_behavior_tree;

enum k_behavior_tree_node_type {
    K_BT_NODE_ROOT,
    K_BT_NODE_ACTION,
};

struct k_behavior_tree_node {

    struct k_list_node child_list_node;

    enum k_behavior_tree_node_type type;

    struct k_behavior_tree *tree;
};

#endif
