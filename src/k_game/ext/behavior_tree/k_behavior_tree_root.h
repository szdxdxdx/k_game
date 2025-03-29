#ifndef K_BEHAVIOR_TREE_ROOT_H
#define K_BEHAVIOR_TREE_ROOT_H

#include "./k_behavior_tree_node.h"

struct k_behavior_tree_root_node {

    struct k_behavior_tree_node base;

    struct k_list child_list;
};

void k__behavior_tree_root_node_init(struct k_behavior_tree_root_node *node);

void k__behavior_tree_root_node_fini(struct k_behavior_tree_root_node *node);

#endif
