#ifndef K_BEHAVIOR_TREE_H
#define K_BEHAVIOR_TREE_H

/**
 * \brief 行为树
 *
 * 行为树是一种用于控制游戏 AI 决策的树状数据结构，
 * 它将复杂的 AI 逻辑分解为模块化的节点，通过树形结构组织决策流程。
 *
 * 行为树执行一次更新操作，为一个先自上而下，再自下而上的过程。行为树产生一个 tick 信号，
 * 先从根节点开始沿着树的结构层层传递，驱动各个节点按照既定逻辑执行自己的任务。
 *
 * 本文档中，tick 一词即可作为名词，表示传播的信号，也可作为动词，表示传播该信号。
 *
 * 不同类型的节点收到 tick 时会做出不同反应：叶子节点会执行具体的动作或检查条件并返回结果，
 * 装饰节点会修改子节点的行为或结果，控制节点会按指定规则继续 tick 它的子节点。
 *
 * 每个节点完成任务后，都会向上返回一个状态码，可能是 SUCCESS、FAILURE 或者 RUNNING。
 * 父节点根据子节点返回的状态码决定后续操作。比如，[sequence] 节点会按顺序 tick 子节点，
 * 所有子节点都返回 SUCCESS 后才返回 SUCCESS，若某个子节点返回 FAILURE 则返回 FAILURE。
 * 而 [selector] 节点则是只要有一个子节点返回 SUCCESS 就立即返回 SUCCESS。
 *
 * 若某个动作需要较长时间完成（比如角色移动，或是等待冷却时间），对应的节点会返回 RUNNING，
 * 表示自己还没结束，下一次 tick 时会继续执行，而不是重新开始。
 *
 * 行为树的把游戏 AI 的决策逻辑拆解成一个个小模块，每个模块都有自己明确的任务。
 * 行为树把 if-else 的判断和执行拆分开来，变成可以单独设计、任意组装的小模块，
 * 内置的 RUNNING 状态码机制使得需要 while 循环执行的耗时任务能被分步分帧执行。
 *
 * k_behavior_tree 的本质其实是一个 k_object。它没有外观，仅用于控制游戏的运作。
 *
 * TODO k_behavior_tree
 *
 * k_behavior_tree 不是一个功能完善的行为树模块，它仅实现了最基本的功能，
 * 自我评价，最基本的功能实现得也不是很好。不过我也琢磨好久了，能写成现在这个样子也很不容易了。
 * 毕设结题在即，有不少模块还没实现，行为树模块就先做成这样子，等过了毕设再完善吧。
 *
 * 尚未实现的功能：
 * - 【黑板】，共享数据存储，类似于一个全局的键值对数据库。
 * - 【优先中断】，允许高优先级行为立即中断低优先级行为的执行，确保 AI 能及时处理紧急事件。
 * - 【动态更新】，允许动态组装各个节点，实现子树复用等。
 * - 【调试】，允许可视化的查看行为树执行的过程，用于调试。
 */
struct k_behavior_tree;

/** \brief 行为树节点的执行结果状态码 */
enum k_behavior_tree_status {
    K_BT_FAILURE = 0,
    K_BT_SUCCESS = 1,
    K_BT_RUNNING = 2,
};

/* region [tree_create] */

/**
 * \brief 创建行为树
 *
 * 若成功，函数返回行为树的指针，否则返回 `NULL`。
 *
 * 初始时的行为树是一颗空树，你需要手动添加节点，逐步构建完整的行为树。
 * 构建操作必须从根节点开始，通过 `k_behavior_tree_get_root()` 获取根节点，
 * 然后使用 `k_behavior_tree_add_XXX()` 系列的函数添加 `XXX` 类型的节点。
 *
 * 行为树的另一种构建方式（builder 模式）详见 `k_behavior_tree_builder`。
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

/** \brief 行为树的节点 */
struct k_behavior_tree_node;

/**
 * \brief 获取行为树的根节点
 *
 * 函数返回行为树的根节点。行为树的构建操作必须从根节点开始。
 */
struct k_behavior_tree_node *k_behavior_tree_get_root(struct k_behavior_tree *tree);

/* region [execution] */

/**
 * \brief 向行为树添加一个 [action] 节点
 *
 * 向 `node` 添加一个 [action] 子节点。若添加成功，函数返回 [action] 节点的指针，否则返回 `NULL`。
 *
 * [action] 在它的每一次 tick 中都会执行一次 `fn_tick()` 回调，
 * 你需要在 `fn_tick()` 中执行实际的行为动作，并返回该动作的执行结果：
 * - 若动作完成，且执行成功，`fn_tick()` 应返回 `K_BT_SUCCESS`。
 * - 若动作完成，且执行失败，`fn_tick()` 应返回 `K_BT_FAILURE`。
 * - 若动作还需要更多次 tick 才能得到结果，应返回 `K_BT_RUNNING`。
 *
 * `data` 为用户自定义数据，你可以在 `fn_tick()` 中使用它。
 *
 * 无法给 [action] 添加子节点。
 */
struct k_behavior_tree_node *k_behavior_tree_add_action(struct k_behavior_tree_node *node, void *data, enum k_behavior_tree_status (*fn_tick)(void *data), void (*fn_interrupt)(void *data));

/**
 * \brief 向行为树添加一个 [condition] 节点
 *
 * 向 `node` 添加一个 [condition] 子节点。若添加成功，函数返回 [condition] 节点的指针，否则返回 `NULL`。
 *
 * [condition] 在它的每一次 tick 中都会执行一次 fn_check() 回调，
 * 你需要在 `fn_check()` 中执行条件检查，并返回该条件的判定结果：
 * - 若条件满足，`fn_check()` 应返回 `K_BT_SUCCESS`。
 * - 若条件不满足，`fn_check()` 应返回 `K_BT_FAILURE`。
 * - `fn_check()` 不应返回 K_BT_RUNNING，因为 [condition] 必须立即给出判定结果。
 *
 * `data` 为用户自定义数据，你可以在 `fn_check()` 中使用它。
 *
 * 无法给 [condition] 添加子节点。
 */
struct k_behavior_tree_node *k_behavior_tree_add_condition(struct k_behavior_tree_node *node, void *data, enum k_behavior_tree_status (*fn_check)(void *data));

/* endregion */

/* region [control] */

/**
 * \brief 向行为树添加一个 [sequence] 节点
 *
 * 向 `node` 添加一个 [sequence] 子节点。若添加成功，函数返回 [sequence] 节点的指针，否则返回 `NULL`。
 *
 * 你可以向 [sequence] 添加任意数量的子节点。[sequence] 将在它的一次 tick 内按顺序依次 tick 子节点。
 *
 * [sequence] 的规则为：
 * - 若当前子节点返回 `K_BT_SUCCESS`，则 [sequence] 立即 tick 下一个子节点。
 *   若所有子节点都返回 `K_BT_SUCCESS`，则 [sequence] 返回 `K_BT_SUCCESS`。
 * - 若当前子节点返回 `K_BT_FAILURE`，则 [sequence] 终止后续 tick，并返回 `K_BT_FAILURE`。
 * - 若当前子节点返回 `K_BT_RUNNING`，则 [sequence] 返回 `K_BT_RUNNING`，之后继续 tick 该子节点。
 * - 若 [sequence] 没有子节点，则返回 `K_BT_SUCCESS`。
 */
struct k_behavior_tree_node *k_behavior_tree_add_sequence(struct k_behavior_tree_node *node);

/**
 * \brief 向行为树添加一个 [selector] 节点
 *
 * 向 `node` 添加一个 [selector] 子节点。若添加成功，函数返回 [selector] 节点的指针，否则返回 `NULL`。
 *
 * 你可以向 [selector] 添加任意数量的子节点。[selector] 将在它的一次 tick 内按顺序依次 tick 子节点。
 *
 * [selector] 的规则为：
 * - 若当前子节点返回 `K_BT_SUCCESS`，则 [selector] 终止后续 tick，并返回 `K_BT_SUCCESS`。
 * - 若当前子节点返回 `K_BT_FAILURE`，则 [selector] 立即 tick 下一个子节点。
 *   若所有子节点都返回 `K_BT_FAILURE`，则 [selector] 返回 `K_BT_FAILURE`。
 * - 若当前子节点返回 `K_BT_RUNNING`，则 [selector] 返回 `K_BT_RUNNING`，之后继续 tick 该子节点。
 * - 若 [sequence] 没有子节点，则返回 `K_BT_FAILURE`。
 */
struct k_behavior_tree_node *k_behavior_tree_add_selector(struct k_behavior_tree_node *node);

/* TODO
 *
 * [random_selector] tick 子节点的顺序是随机的，其余行为都与 [selector] 相同。
 */
struct k_behavior_tree_node *k_behavior_tree_add_random_selector(struct k_behavior_tree_node *node);

/**
 * \brief 向行为树添加一个 [parallel] 节点
 *
 * 向 `node` 添加一个 [parallel] 子节点。若添加成功，函数返回 [parallel] 节点的指针，否则返回 `NULL`。
 *
 * 你可以向 [parallel] 添加任意数量的子节点。[parallel] 将尝试在它的一次 tick 内 tick 它的多个子节点。
 *
 * `success_policy` 和 `failure_policy` 分别表示 [parallel] 返回 `K_BT_SUCCESS` 和 `K_BT_FAILURE` 的策略。
 * 要求 `success_policy` 和 `failure_policy` 的值必须为正数。
 *
 * [parallel] 的规则为：
 * - 若一部分子节点处于 RUNNING 或未开始执行动作，而另一部分子节点的动作已执行完毕：
 *    - 若返回 `K_BT_SUCCESS` 的子节点个数达到 `success_policy`，则 [parallel] 中断所有 RUNNING 的子节点，并返回 `K_BT_SUCCESS`。
 *    - 若返回 `K_BT_FAILURE` 的子节点个数达到 `failure_policy`，则 [parallel] 中断所有 RUNNING 的子节点，并返回 `K_BT_FAILURE`。
 *    - 否则 [parallel] 返回 `K_BT_RUNNING`，下一次 tick 时，[parallel] 继续 tick 处于 RUNNING 的子节点。
 * - 若所有子结点的动作都执行完毕：若所有子结点都返回 `K_BT_SUCCESS`，则 [parallel] 返回 `K_BT_SUCCESS`，否则返回 `K_BT_FAILURE`。
 * - 若 [parallel] 没有子节点，则返回 `K_BT_SUCCESS`。
 *
 * 依照上述的规则，`success_policy` 和 `failure_policy`的几种可能的取值为：
 * - 若 `success_policy` 为 `INT_MAX`，则表示：全部子结点都返回 `K_BT_SUCCESS`，[parallel] 才返回 `K_BT_SUCCESS`。
 * - 若 `success_policy` 为 `1`，则表示：任意子结点都返回 `K_BT_SUCCESS`，[parallel] 就返回 `K_BT_SUCCESS`。
 * - 若 `failure_policy` 为 `INT_MAX`，则表示：等待所有子结点的动作都执行完毕，若有的子结点返回 `K_BT_FAILURE`，则 [parallel] 返回 `K_BT_FAILURE`。
 * - 若 `failure_policy` 为 `1`，则表示：任意子结点都返回 `K_BT_FAILURE`，[parallel] 就返回 `K_BT_FAILURE`。
 */
struct k_behavior_tree_node *k_behavior_tree_add_parallel(struct k_behavior_tree_node *node, int success_policy, int failure_policy);

/* endregion */

/* region [decorator] */

/**
 * \brief 向行为树添加一个 [inverter] 节点
 *
 * 向 `node` 添加一个 [inverter] 子节点。若添加成功，函数返回 [inverter] 节点的指针，否则返回 `NULL`。
 *
 * [inverter] 最多只能有一个子节点。[inverter] 将翻转子节点的返回状态。
 *
 * [inverter] 的规则为：
 * - 若子节点返回 `K_BT_SUCCESS`，则 [inverter] 返回 `K_BT_FAILURE`。
 * - 若子节点返回 `K_BT_FAILURE`，则 [inverter] 返回 `K_BT_SUCCESS`。
 * - 若子节点返回 `K_BT_RUNNING`，则 [inverter] 返回 `K_BT_RUNNING`。
 * - 若 [inverter] 没有子节点，则返回 `K_BT_SUCCESS`。
 */
struct k_behavior_tree_node *k_behavior_tree_add_inverter(struct k_behavior_tree_node *node);

/**
 * \brief 向行为树添加一个 [force_success] 节点
 *
 * 向 `node` 添加一个 [force_success] 子节点。若添加成功，函数返回 [force_success] 节点的指针，否则返回 `NULL`。
 *
 * [force_success] 最多只能有一个子节点。[force_success] 将强制修正子结点的返回状态为 `K_BT_SUCCESS`。
 *
 * [force_success] 的规则为：
 * - 若子节点返回 `K_BT_RUNNING`，则 [force_success] 返回 `K_BT_RUNNING`，否则返回 `K_BT_SUCCESS`。
 * - 若 [force_success] 没有子节点，则返回 `K_BT_SUCCESS`。
 */
struct k_behavior_tree_node *k_behavior_tree_add_force_success(struct k_behavior_tree_node *node);

/**
 * \brief 向行为树添加一个 [force_failure] 节点
 *
 * 向 `node` 添加一个 [force_failure] 子节点。若添加成功，函数返回 [force_failure] 节点的指针，否则返回 `NULL`。
 *
 * [force_failure] 最多只能有一个子节点。[force_failure] 将强制修正子结点的返回状态为 `K_BT_FAILURE`。
 *
 * [force_failure] 的规则为：
 * - 若子节点返回 `K_BT_RUNNING`，则 [force_failure] 返回 `K_BT_RUNNING`，否则返回 `K_BT_FAILURE`。
 * - 若 [force_failure] 没有子节点，则返回 `K_BT_FAILURE`。
 */
struct k_behavior_tree_node *k_behavior_tree_add_force_failure(struct k_behavior_tree_node *node);

/**
 * \brief 向行为树添加一个 [repeat] 节点
 *
 * 向 `node` 添加一个 [repeat] 子节点。若添加成功，函数返回 [repeat] 节点的指针，否则返回 `NULL`。
 *
 * [repeat] 最多只能有一个子节点。[repeat] 将在它的一次 tick 内重复 tick 它的子节点，
 * 直到子节点返回 `K_BT_FAILURE`，或重复次数到达 `n` 次。
 *
 * [repeat] 的规则为：
 * - 若子节点每次都返回 `K_BT_SUCCESS`，且完成了 `n` 次，则 [repeat] 重置计数器，并返回 `K_BT_SUCCESS`。
 * - 若子节点任意一次返回 `K_BT_FAILURE`，则 [repeat] 终止后续 tick，重置计数器，并返回 `K_BT_FAILURE`。
 * - 若子节点返回 `K_BT_RUNNING`，则 [repeat] 暂停计数，并返回 `K_BT_RUNNING`。
 * - 若 [repeat] 没有子节点，则返回 `K_BT_SUCCESS`。
 */
struct k_behavior_tree_node *k_behavior_tree_add_repeat(struct k_behavior_tree_node *node, size_t n);

/* TODO
 *
 * [retry] 将在它的一次 tick 内重复 tick 它的子节点，直到子节点返回 `K_BT_SUCCESS`，或重复次数到达 `n` 次。
 */
struct k_behavior_tree_node *k_behavior_tree_add_retry(struct k_behavior_tree_node *node, size_t n);

/* TODO
 *
 * 若子节点未能在 timeout_ms 时间内返回 `K_BT_SUCCESS`，则 [timeout] 中断子节点，并返回 `K_BT_FAILURE`。
 */
struct k_behavior_tree_node *k_behavior_tree_add_timeout(struct k_behavior_tree_node *node, int timeout_ms);

/**
 * \brief 向行为树添加一个 [delay] 节点
 *
 * 向 `node` 添加一个 [delay] 子节点。若添加成功，函数返回 [delay] 节点的指针，否则返回 `NULL`。
 *
 * [delay] 最多只能有一个子节点。[delay] 节点将延迟 tick 它的子节点。
 *
 * [delay] 的规则为：
 * - 若 [delay] 已启动计时器，但未达到设定的延迟时间，则不 tick 子节点，并返回 `K_BT_RUNNING`。
 * - 若 [delay] 已达到设定的延迟时间，则 tick 子节点，并返回子节点的返回状态。
 *   若子节点返回 `K_BT_SUCCESS` 或 `K_BT_FAILURE` 时，[delay] 重置。
 * - 若 [delay] 没有子节点，则延迟后返回 `K_BT_SUCCESS`。
 */
struct k_behavior_tree_node *k_behavior_tree_add_delay(struct k_behavior_tree_node *node, int delay_ms);

/* endregion */

/* endregion */

/* region [tree_builder] */

/**
 * \brief 行为树构建器
 *
 * 构建器用于简化行为树的构建过程，通过代码块和缩进表达节点间的层次关系，代码布局直接反映树的结构。
 * 通过 `k_bt_builder()` 宏初始化构建器，在代码块内使用各种 `k_bt_XXX()` 系列的宏添加节点。
 *
 * 示例：
 * ```C
 * struct k_behavior_tree *tree = NULL;
 *
 * struct k_behavior_tree_builder *b;       // 定义构建器的指针，用于存储上下文
 * k_bt_builder(&tree, b)                   // 开始构建。`tree` 用于接收结果
 * {                                        // 当前上下文为 [root]
 *     k_bt_parallel(b)                     // 向 [root] 添加一个 [parallel]
 *     {                                    // 当前上下文为 [parallel]
 *         k_bt_action(b, ...);             // 向 [parallel] 添加一个 [action]
 *         k_bt_sequence(b)                 // 向 [parallel] 添加一个 [sequence]
 *         {                                // 当前上下文为 [sequence]
 *             k_bt_condition(b, ...);      // 向 [sequence] 添加一个 [condition]
 *             k_bt_repeat(b, ...)          // 向 [sequence] 添加一个 [repeat]
 *             {                            // 当前上下文为 [repeat]
 *                 k_bt_delay(b, ...)       // 向 [repeat] 添加一个 [delay]
 *                 {                        // 当前上下文为 [delay]
 *                     k_bt_action(b, ...); // 向 [delay] 添加一个 [action]
 *                 }                        // 返回到上一级 [repeat]
 *                 k_bt_action(b, ...);     // 向 [repeat] 添加一个 [action]
 *             }                            // 返回到上一级 [sequence]
 *         }                                // 返回到上一级 [parallel]
 *         k_bt_action(b, ...);             // 向 [parallel] 添加一个 [action]
 *     }                                    // 返回到上一级 [root]
 * }                                        // 结束构建，构建器自动销毁
 *
 * if (NULL != tree) {   // 若失败，构建器会销毁行为树，并往 `tree` 中写入 `NULL`
 *     goto err;         // 若中途某步添加节点失败，则算失败，所有节点都添加成功才算成功
 * }
 * ```
 *
 * 注意：不要在构建器的代码块内使用 return、break、goto 等语句，否则构建器将无法正确维护内部状态。
 */
struct k_behavior_tree_builder;

/* region */

/* 【请忽略当前 region】若要使用 builder 模式构造行为树，请使用下方 region [builder] 中的宏 */

struct k_behavior_tree_builder *k__behavior_tree_builder_begin(struct k_behavior_tree **get_tree);
int  k__behavior_tree_builder_end(struct k_behavior_tree_builder *builder);
int  k__behavior_tree_builder_pop(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_action(struct k_behavior_tree_builder *builder, void *data, enum k_behavior_tree_status (*fn_tick)(void *data), void (*fn_interrupt)(void *data));
void k__behavior_tree_builder_condition(struct k_behavior_tree_builder *builder, void *data, enum k_behavior_tree_status (*fn_check)(void *data));
void k__behavior_tree_builder_sequence(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_selector(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_parallel(struct k_behavior_tree_builder *builder, int success_policy, int failure_policy);
void k__behavior_tree_builder_inverter(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_force_success(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_force_failure(struct k_behavior_tree_builder *builder);
void k__behavior_tree_builder_repeat(struct k_behavior_tree_builder *builder, size_t n);
void k__behavior_tree_builder_retry(struct k_behavior_tree_builder *builder, size_t n);         /* <- TODO */
void k__behavior_tree_builder_timeout(struct k_behavior_tree_builder *builder, int timeout_ms); /* <- TODO */
void k__behavior_tree_builder_delay(struct k_behavior_tree_builder *builder, int delay_ms);

/* endregion */

/* region [builder] */

#define k_bt_builder(tree, builder) \
    for (builder = k__behavior_tree_builder_begin(tree); NULL != builder && k__behavior_tree_builder_end(builder); )

#define k_bt_action(builder, data, fn_tick, fn_interrupt) \
    k__behavior_tree_builder_action(builder, data, fn_tick, fn_interrupt)

#define k_bt_condition(builder, data, fn_check) \
    k__behavior_tree_builder_condition(builder, data, fn_check)

#define k_bt_sequence(builder) \
    for (k__behavior_tree_builder_sequence(builder); k__behavior_tree_builder_pop(builder); )

#define k_bt_selector(builder) \
    for (k__behavior_tree_builder_selector(builder); k__behavior_tree_builder_pop(builder); )

#define k_bt_parallel(builder, success_policy, failure_policy) \
    for (k__behavior_tree_builder_parallel(builder, success_policy, failure_policy); k__behavior_tree_builder_pop(builder); )

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

/* region [tree_get/set] */

/* TODO */
void k_behavior_tree_set_destroy_callback(struct k_behavior_tree *tree, void (*fn_callback)(struct k_behavior_tree *tree));

/* endregion */

#endif
