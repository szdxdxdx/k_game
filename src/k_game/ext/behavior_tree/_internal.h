#ifndef K_BEHAVIOR_TREE_INTERNAL_H
#define K_BEHAVIOR_TREE_INTERNAL_H

#include "k_array.h"
#include "k_game.h"

#include "./_public.h"

struct k_behavior_tree_node {

    struct k_behavior_tree *tree;

    enum k_behavior_tree_status (*fn_tick)(struct k_behavior_tree_node *node);

    int (*fn_add)(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node);

    void (*fn_destroy)(struct k_behavior_tree_node *node);
};

#endif
