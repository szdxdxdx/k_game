#ifndef K_BEHAVIOR_TREE_INTERNAL_H
#define K_BEHAVIOR_TREE_INTERNAL_H

#include "k_game.h"

struct k_behavior_tree_node {

    struct k_behavior_tree *tree;

    enum k_behavior_tree_status (*fn_tick)(struct k_behavior_tree_node *node);

    void (*fn_interrupt)(struct k_behavior_tree_node *node);

    int (*fn_add_child)(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node);

    void (*fn_destroy)(struct k_behavior_tree_node *node);
};

extern struct k_behavior_tree_node K__BEHAVIOR_TREE_DEFAULT_SUCCESS;

extern struct k_behavior_tree_node K__BEHAVIOR_TREE_DEFAULT_FAILURE;

#endif
