#ifndef K__BEHAVIOR_TREE_INTERNAL_H
#define K__BEHAVIOR_TREE_INTERNAL_H

#include <stddef.h>

#include "k_game/ext/k_behavior_tree.h"

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
#endif

struct k_behavior_tree_node {

    struct k_behavior_tree *tree;

    enum k_behavior_tree_status (*fn_tick)(struct k_behavior_tree_node *node);

    /* 中断运行，重置节点状态。应确保中断函数的实现是幂等的 */
    void (*fn_interrupt)(struct k_behavior_tree_node *node);

    /* 添加子节点。若添加成功，函数应返回 0，否则应返回非 0 */
    int (*fn_add_child)(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node);

    /* 用于销毁节点自身 */
    void (*fn_destroy)(struct k_behavior_tree_node *node);
};

/* 等效于 `NULL` 的默认节点，`fn_tick()` 始终返回成功，或始终返回失败 */
extern struct k_behavior_tree_node K__BEHAVIOR_TREE_DEFAULT_SUCCESS;
extern struct k_behavior_tree_node K__BEHAVIOR_TREE_DEFAULT_FAILURE;

#endif
