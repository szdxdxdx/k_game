#ifndef K_BEHAVIOR_TREE_ACTION_H
#define K_BEHAVIOR_TREE_ACTION_H

#include "./k_behavior_tree_node.h"

struct k_object;

struct k_behavior_tree_action_node {

    struct k_behavior_tree_node base;

    enum k_behavior_tree_state (*fn_tick)(struct k_object *object);
};

#endif
