#ifndef K_BEHAVIOR_TREE_H
#define K_BEHAVIOR_TREE_H

struct k_object;

/**
 * \brief 行为树
 *
 * TODO docs
 */
struct k_behavior_tree;

/* region [object_add_behavior_tree] */

struct k_behavior_tree *k_object_add_behavior_tree(struct k_object *object);

void k_object_del_behavior_tree(struct k_behavior_tree *node);

/* endregion */

/* region [build_tree] */

struct k_behavior_tree_node;

struct k_behavior_tree_node *k_behavior_tree_get_root(struct k_behavior_tree *tree);

/* region [execution] */

enum k_behavior_tree_state {
    K_BT_FAILURE = -1,
    K_BT_SUCCESS =  0,
    K_BT_RUNNING =  1,
};

struct k_behavior_tree_node *k_behavior_tree_add_action(struct k_behavior_tree_node *node, enum k_behavior_tree_state (*fn_tick)(struct k_object *object));

struct k_behavior_tree_node *k_behavior_tree_add_condition(struct k_behavior_tree_node *node, enum k_behavior_tree_state (*fn_tick)(struct k_object *object));

/* endregion */

/* region [control] */

struct k_behavior_tree_node *k_behavior_tree_add_selector(struct k_behavior_tree_node *node);

struct k_behavior_tree_node *k_behavior_tree_add_sequence(struct k_behavior_tree_node *node);

struct k_behavior_tree_node *k_behavior_tree_add_parallel(struct k_behavior_tree_node *node);

struct k_behavior_tree_node *k_behavior_tree_add_random(struct k_behavior_tree_node *node);

/* endregion */

/* region [decorator] */

struct k_behavior_tree_node *k_behavior_tree_add_repeat(struct k_behavior_tree_node *node);

struct k_behavior_tree_node *k_behavior_tree_add_timeout(struct k_behavior_tree_node *node);

struct k_behavior_tree_node *k_behavior_tree_add_inverter(struct k_behavior_tree_node *node);

/* endregion */

/* endregion */

#endif
