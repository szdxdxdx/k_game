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

/* region [build_behavior_tree] */

struct k_bt_node;

struct k_bt_node *k_bt_get_root(struct k_behavior_tree *tree);

/* region [execution] */

enum k_bt_state {
    K_BT_SUCCESS,
    K_BT_FAILURE,
    K_BT_RUNNING,
};

struct k_bt_node *k_bt_add_action(struct k_bt_node *node, enum k_bt_state (*fn_tick)(struct k_object *object));

struct k_bt_node *k_bt_add_condition(struct k_bt_node *node, enum k_bt_state (*fn_tick)(struct k_object *object));

/* endregion */

/* region [control] */

struct k_bt_node *k_bt_add_selector(struct k_bt_node *node);

struct k_bt_node *k_bt_add_sequence(struct k_bt_node *node);

struct k_bt_node *k_bt_add_parallel(struct k_bt_node *node);

struct k_bt_node *k_bt_add_random(struct k_bt_node *node);

/* endregion */

/* region [decorator] */

struct k_bt_node *k_bt_add_repeat(struct k_bt_node *node);

struct k_bt_node *k_bt_add_timeout(struct k_bt_node *node);

struct k_bt_node *k_bt_add_inverter(struct k_bt_node *node);

/* endregion */

/* endregion */

#endif
