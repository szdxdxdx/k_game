#include "./k_behavior_tree_internal.h"

static enum k_behavior_tree_status success_tick(struct k_behavior_tree_node *node) {
    (void)node;
    return K_BT_SUCCESS;
}

static enum k_behavior_tree_status failure_tick(struct k_behavior_tree_node *node) {
    (void)node;
    return K_BT_FAILURE;
}

static void default_interrupt(struct k_behavior_tree_node *node) {
    (void)node;
}

static void default_destroy(struct k_behavior_tree_node *node) {
    (void)node;
}

struct k_behavior_tree_node K__BEHAVIOR_TREE_DEFAULT_SUCCESS = {
    .fn_tick      = success_tick,
    .fn_interrupt = default_interrupt,
    .fn_add_child = NULL,
    .fn_destroy   = default_destroy,
};

struct k_behavior_tree_node K__BEHAVIOR_TREE_DEFAULT_FAILURE = {
    .fn_tick      = failure_tick,
    .fn_interrupt = default_interrupt,
    .fn_add_child = NULL,
    .fn_destroy   = default_destroy,
};
