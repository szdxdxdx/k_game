#ifndef K_BEHAVIOR_TREE__INTERNAL_H
#define K_BEHAVIOR_TREE__INTERNAL_H

#include "k_game.h"

#include "./_public.h"

/* region [node] */

enum k_bt_node_type {
    K_BT_NOTE_ACTION,
};

struct k_bt_node {
    enum k_bt_node_type type;
};

struct k_bt_action_node {
    struct k_bt_node base;
    enum k_bt_state (*fn_tick)(struct k_object *object);
};

/* endregion */

struct k_behavior_tree {

    struct k_component *component;

    struct k_bt_node *root;
};

#endif
