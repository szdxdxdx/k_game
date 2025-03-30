#include "./_internal.h"

static enum k_behavior_tree_status default_success(struct k_behavior_tree_node *node) {
    (void)node;
    return K_BT_SUCCESS;
}

static enum k_behavior_tree_status default_failure(struct k_behavior_tree_node *node) {
    (void)node;
    return K_BT_FAILURE;
}

static void default_destroy(struct k_behavior_tree_node *node) {
    (void)node;
}

struct k_behavior_tree_node K__BEHAVIOR_TREE_DEFAULT_SUCCESS = {
    .fn_interrupt = NULL,
    .fn_tick      = default_success,
    .fn_add_child = NULL,
    .fn_destroy   = default_destroy,
};

struct k_behavior_tree_node K__BEHAVIOR_TREE_DEFAULT_FAILURE = {
    .fn_interrupt = NULL,
    .fn_tick      = default_failure,
    .fn_add_child = NULL,
    .fn_destroy   = default_destroy,
};
