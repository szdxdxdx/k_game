#ifndef K_BEHAVIOR_TREE__INTERNAL_H
#define K_BEHAVIOR_TREE__INTERNAL_H

#include "k_game.h"

#include "./k_behavior_tree_root.h"
#include "./k_behavior_tree_action.h"

#include "./_public.h"

struct k_behavior_tree {

    struct k_component *component;

    struct k_behavior_tree_root_node root;
};

#endif
