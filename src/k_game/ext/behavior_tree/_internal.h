#ifndef K_BEHAVIOR_TREE_INTERNAL_H
#define K_BEHAVIOR_TREE_INTERNAL_H

#include "k_game.h"

struct k_behavior_tree_node {

    struct k_behavior_tree *tree;

    enum k_behavior_tree_status (*fn_tick)(struct k_behavior_tree_node *node);

    /* 中断运行，重置结点状态。应确保中断函数的实现是幂等的 */
    void (*fn_interrupt)(struct k_behavior_tree_node *node);

    /* 添加子结点。若添加成功，函数应返回 0，否则应返回非 0 */
    int (*fn_add_child)(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node);

    /* 用于销毁结点自身 */
    void (*fn_destroy)(struct k_behavior_tree_node *node);
};

extern struct k_behavior_tree_node K__BEHAVIOR_TREE_DEFAULT_SUCCESS;

extern struct k_behavior_tree_node K__BEHAVIOR_TREE_DEFAULT_FAILURE;

#endif
