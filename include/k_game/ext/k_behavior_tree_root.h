#ifndef K_BEHAVIOR_TREE_ROOT_H
#define K_BEHAVIOR_TREE_ROOT_H

struct k_behavior_tree;

struct k_behavior_tree *k_behavior_tree_create(void);

void k_behavior_tree_destroy(struct k_behavior_tree *tree);

struct k_behavior_tree_node;

struct k_behavior_tree_node *k_behavior_tree_get_root(struct k_behavior_tree *tree);

/* region [action] */

enum k_behavior_tree_status {
    K_BT_FAILURE = 0,
    K_BT_SUCCESS = 1,
    K_BT_RUNNING = 2,
};

struct k_behavior_tree_node *k_behavior_tree_add_action(struct k_behavior_tree_node *node, void *data, enum k_behavior_tree_status (*fn_tick)(void *data));

struct k_behavior_tree_node *k_behavior_tree_add_condition(struct k_behavior_tree_node *node, void *data, enum k_behavior_tree_status (*fn_tick)(void *data));

/* endregion */

/* region [control] */

struct k_behavior_tree_node *k_behavior_tree_add_sequence(struct k_behavior_tree_node *node);

/* endregion */

#endif
