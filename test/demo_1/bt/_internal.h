#ifndef YX_BEHAVIOR_TREE_INTERNAL_H
#define YX_BEHAVIOR_TREE_INTERNAL_H

#include "k_array.h"
#include "k_game.h"

#include "./_public.h"

struct k_behavior_tree;

/* region [node] */

enum yx_behavior_tree_status {
    YX_BT_FAILURE = -1,
    YX_BT_SUCCESS =  0,
    YX_BT_RUNNING =  1,
};

struct yx_behavior_tree_node {

    struct yx_behavior_tree *tree;

    enum yx_behavior_tree_status (*fn_tick)(struct yx_behavior_tree_node *node);

    int (*fn_add)(struct yx_behavior_tree_node *node, struct yx_behavior_tree_node *child_node);

    void (*fn_destroy)(struct yx_behavior_tree_node *node);
};

/* region [action] */

struct yx_behavior_tree_action_node {

    struct yx_behavior_tree_node super;

    enum yx_behavior_tree_status (*fn_tick)(void *data);

    void *data;
};

struct yx_behavior_tree_node *yx_behavior_tree_add_action(struct yx_behavior_tree_node *node, void *data, enum yx_behavior_tree_status (*fn_tick)(void *data));

/* endregion */

/* region [control] */

struct yx_behavior_tree_sequence_node {

    struct yx_behavior_tree_node super;

    struct k_array children;

    size_t index;
};

struct yx_behavior_tree_node *yx_behavior_tree_add_sequence(struct yx_behavior_tree_node *node);

/* endregion */

/* endregion */

/* region [tree] */

struct yx_behavior_tree_root_node {

    struct yx_behavior_tree_node super;

    struct yx_behavior_tree_node *child;
};

struct yx_behavior_tree {

    struct yx_behavior_tree_root_node root;

    struct k_object *object;
};

struct yx_behavior_tree_node *yx_behavior_tree_get_root(struct yx_behavior_tree *tree);

/* endregion */

#endif
