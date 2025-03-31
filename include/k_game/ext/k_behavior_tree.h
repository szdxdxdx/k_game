#ifndef K_BEHAVIOR_TREE_H
#define K_BEHAVIOR_TREE_H

struct k_behavior_tree;

/* region [tree_create] */

struct k_behavior_tree *k_behavior_tree_create(void);

void k_behavior_tree_destroy(struct k_behavior_tree *tree);

/* endregion */

/* region [tree_add_node] */

struct k_behavior_tree_node;

struct k_behavior_tree_node *k_behavior_tree_get_root(struct k_behavior_tree *tree);

/* region [execution] */

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

struct k_behavior_tree_node *k_behavior_tree_add_parallel(struct k_behavior_tree_node *node);

/* endregion */

/* region [decorator] */

struct k_behavior_tree_node *k_behavior_tree_add_inverter(struct k_behavior_tree_node *node);

struct k_behavior_tree_node *k_behavior_tree_add_force_success(struct k_behavior_tree_node *node);

struct k_behavior_tree_node *k_behavior_tree_add_force_failure(struct k_behavior_tree_node *node);

/**
 * \brief 向行为树添加一个【重复器】结点
 *
 * 【重复器】repeater 结点将尝试在它的一次 tick 内，多次 tick 它的子结点。
 *
 * - 若子结点每次都返回 SUCCESS，且完成了 `n` 次，则 repeater 重置计数器，并返回 SUCCESS。
 * - 若子结点任意一次返回 FAILURE，则 repeater 终止后续 tick，重置计数器，并返回 FAILURE。
 * - 若子结点返回 RUNNING，则 repeater 暂停计数，并返回 RUNNING。
 *
 * repeater 最多只能有一个子结点。若 repeater 没有子结点，则每次 tick 都返回 SUCCESS。
 */
struct k_behavior_tree_node *k_behavior_tree_add_repeater(struct k_behavior_tree_node *node, size_t n);

/* endregion */

/* endregion */

/* region [tree_builder] */

struct k_behavior_tree_builder;

/* region */

/* 【请忽略当前 region】若要使用 builder 模式构造行为树，请使用下方 region [builder] 中的宏 */

struct k_behavior_tree_builder *k__behavior_tree_builder_begin(struct k_behavior_tree **get_tree);
int  k__behavior_tree_builder_end(struct k_behavior_tree_builder *builder);
int  k__behavior_tree_builder_pop(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_action(struct k_behavior_tree_builder *builder, void *data, enum k_behavior_tree_status (*fn_tick)(void *data));
void k__behavior_tree_builder_condition(struct k_behavior_tree_builder *builder, void *data, enum k_behavior_tree_status (*fn_tick)(void *data));
void k__behavior_tree_builder_sequence(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_selector(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_parallel(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_inverter(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_force_success(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_force_failure(struct k_behavior_tree_builder *builder);

/* endregion */

/* region [builder] */

#define k_bt_builder(tree, builder) \
    for (builder = k__behavior_tree_builder_begin(tree); NULL != builder && k__behavior_tree_builder_end(builder); )

#define k_bt_action(builder, data, fn_tick) \
    k__behavior_tree_builder_action(builder, data, fn_tick)

#define k_bt_condition(builder, data, fn_tick) \
    k__behavior_tree_builder_condition(builder, data, fn_tick)

#define k_bt_sequence(builder) \
    for (k__behavior_tree_builder_sequence(builder); k__behavior_tree_builder_pop(builder); )

#define k_bt_selector(builder) \
    for (k__behavior_tree_builder_selector(builder); k__behavior_tree_builder_pop(builder); )

#define k_bt_parallel(builder) \
    for (k__behavior_tree_builder_parallel(builder); k__behavior_tree_builder_pop(builder); )

#define k_bt_inverter(builder) \
    for (k__behavior_tree_builder_inverter(builder); k__behavior_tree_builder_pop(builder); )

#define k_bt_force_success(builder) \
    for (k__behavior_tree_builder_force_success(builder); k__behavior_tree_builder_pop(builder); )

#define k_bt_force_failure(builder) \
    for (k__behavior_tree_builder_force_failure(builder); k__behavior_tree_builder_pop(builder); )

/* endregion */

/* endregion */

#endif
