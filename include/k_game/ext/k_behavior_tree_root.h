#ifndef K_BEHAVIOR_TREE_ROOT_H
#define K_BEHAVIOR_TREE_ROOT_H

struct k_behavior_tree;

/* region [tree_create] */

struct k_behavior_tree *k_behavior_tree_create(void);

void k_behavior_tree_destroy(struct k_behavior_tree *tree);

/* endregion */

/* region [tree_add_node] */

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

struct k_behavior_tree_node *k_behavior_tree_add_selector(struct k_behavior_tree_node *node);

/* endregion */

/* region [decorator] */

struct k_behavior_tree_node *k_behavior_tree_add_inverter(struct k_behavior_tree_node *node);

/* endregion */

/* endregion */

/* region [tree_builder] */

struct k_behavior_tree_builder;

/* region [ignore] */

struct k_behavior_tree_builder *k__behavior_tree_builder_begin(struct k_behavior_tree *tree);
int k__behavior_tree_builder_is_valid(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_end(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_sequence_begin(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_sequence_end  (struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_selector_begin(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_selector_end  (struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_action        (struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_condition     (struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_inverter_begin(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_inverter_end  (struct k_behavior_tree_builder *builder);

/* endregion */

#define k_bt_builder(tree, builder) \
    for (builder = k__behavior_tree_builder_begin(tree); k__behavior_tree_builder_is_valid(builder); k__behavior_tree_builder_end(builder))

#define k_bt_sequence(builder) \
    for (k__behavior_tree_builder_sequence_begin(builder); k__behavior_tree_builder_is_valid(builder); k__behavior_tree_builder_sequence_end(builder))

#define k_bt_selector(builder) \
    for (k__behavior_tree_builder_selector_begin(builder); k__behavior_tree_builder_is_valid(builder); k__behavior_tree_builder_selector_end(builder))

#define k_bt_action(builder, data, fn_tick) \
    k__behavior_tree_builder_action(builder)

#define k_bt_condition(builder, data, fn_tick) \
    k__behavior_tree_builder_condition(builder)

/* endregion */

#endif
