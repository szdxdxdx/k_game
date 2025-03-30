#ifndef K_BEHAVIOR_TREE_H
#define K_BEHAVIOR_TREE_H

struct k_object;

/**
 * \brief 行为树
 *
 * TODO docs
 */
struct k_behavior_tree;

/* region [behavior_tree_create] */

struct k_behavior_tree *k_behavior_tree_create(void);

void k_behavior_tree_destroy(struct k_behavior_tree *tree);

/* endregion */

/* region [build_tree] */

struct k_behavior_tree_node;

struct k_behavior_tree_node *k_behavior_tree_get_root(struct k_behavior_tree *tree);

/* region [execution] */

enum k_behavior_tree_status {
    K_BT_FAILURE = -1,
    K_BT_SUCCESS =  0,
    K_BT_RUNNING =  1,
};

struct k_behavior_tree_node *k_behavior_tree_add_action(struct k_behavior_tree_node *node);

struct k_behavior_tree_node *k_behavior_tree_add_condition(struct k_behavior_tree_node *node);

/* endregion */

/* region [control] */

struct k_behavior_tree_node *k_behavior_tree_add_sequence(struct k_behavior_tree_node *node);

/* endregion */

/* endregion */

#endif
