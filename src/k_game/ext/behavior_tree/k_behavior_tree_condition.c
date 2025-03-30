#include <stdlib.h>

#include "./_internal.h"

/* region [condition] */

struct k_behavior_tree_condition_node {

    struct k_behavior_tree_node super;

    enum k_behavior_tree_status (*fn_tick)(void *data);

    void *data;
};

static enum k_behavior_tree_status condition_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_condition_node *condition = (struct k_behavior_tree_condition_node *)node;

    return condition->fn_tick(condition->data);
}

static int condition_add_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child) {
    (void)node;
    (void)child;
    return -1;
}

static void condition_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_condition_node *condition = (struct k_behavior_tree_condition_node *)node;
    free(condition);
}

struct k_behavior_tree_node *k_behavior_tree_add_condition(struct k_behavior_tree_node *node, void *data, enum k_behavior_tree_status (*fn_tick)(void *data)) {

    struct k_behavior_tree_condition_node *condition = malloc(sizeof(struct k_behavior_tree_condition_node));
    if (NULL == condition)
        return NULL;

    condition->super.fn_tick    = condition_tick;
    condition->super.fn_add     = condition_add_child;
    condition->super.fn_destroy = condition_destroy;
    condition->super.tree       = node->tree;

    condition->fn_tick = fn_tick;
    condition->data    = data;

    if (0 != node->fn_add(node, &condition->super))
        return NULL;

    return &condition->super;
}
