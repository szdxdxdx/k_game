#ifndef K_BEHAVIOR_TREE_H
#define K_BEHAVIOR_TREE_H

struct k_behavior_tree;

/* region [tree_create] */

/**
 * \brief 创建行为树
 *
 * 若成功，函数返回行为树的指针，否则返回 `NULL`。
 */
struct k_behavior_tree *k_behavior_tree_create(void);

/**
 * \brief 销毁行为树
 *
 * 若 `tree` 为 `NULL`，则函数立即返回。
 */
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
 * \brief 向行为树添加一个 repeat 结点
 *
 * repeat 结点将尝试在它的一次 tick 内，多次 tick 它的子结点。
 *
 * - 若子结点每次都返回 SUCCESS，且完成了 `n` 次，则 repeat 重置计数器，并返回 SUCCESS。
 * - 若子结点任意一次返回 FAILURE，则 repeat 终止后续 tick，重置计数器，并返回 FAILURE。
 * - 若子结点返回 RUNNING，则 repeat 暂停计数，并返回 RUNNING。
 *
 * repeat 最多只能有一个子结点。若 repeat 没有子结点，则每次 tick 都返回 SUCCESS。
 */
struct k_behavior_tree_node *k_behavior_tree_add_repeat(struct k_behavior_tree_node *node, size_t n);

struct k_behavior_tree_node *k_behavior_tree_add_retry(struct k_behavior_tree_node *node, size_t n);

struct k_behavior_tree_node *k_behavior_tree_add_timeout(struct k_behavior_tree_node *node, int timeout_ms);

struct k_behavior_tree_node *k_behavior_tree_add_delay(struct k_behavior_tree_node *node, int delay_ms);

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
void k__behavior_tree_builder_repeat(struct k_behavior_tree_builder *builder, size_t n);
void k__behavior_tree_builder_retry(struct k_behavior_tree_builder *builder, size_t n);
void k__behavior_tree_builder_timeout(struct k_behavior_tree_builder *builder, int timeout_ms);
void k__behavior_tree_builder_delay(struct k_behavior_tree_builder *builder, int delay_ms);

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

#define k_bt_repeat(builder, n) \
    for (k__behavior_tree_builder_repeat(builder, n); k__behavior_tree_builder_pop(builder); )

#define k_bt_retry(builder, n) \
    for (k__behavior_tree_builder_retry(builder, n); k__behavior_tree_builder_pop(builder); )

#define k_bt_timeout(builder, timeout_ms) \
    for (k__behavior_tree_builder_timeout(builder, timeout_ms); k__behavior_tree_builder_pop(builder); )

#define k_bt_delay(builder, delay_ms) \
    for (k__behavior_tree_builder_delay(builder, delay_ms); k__behavior_tree_builder_pop(builder); )

/* endregion */

/* endregion */

#endif
