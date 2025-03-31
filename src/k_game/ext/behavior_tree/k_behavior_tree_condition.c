#include <stdlib.h>

#include "./_internal.h"

struct k_behavior_tree_condition_node {

    struct k_behavior_tree_node super;

    enum k_behavior_tree_status (*fn_tick)(void *data);

    void *data;
};

static enum k_behavior_tree_status condition_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_condition_node *condition = container_of(node, struct k_behavior_tree_condition_node, super);
    return condition->fn_tick(condition->data);
}

static int condition_add_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child) {
    (void)node;
    (void)child;
    return -1;
}

static void condition_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_condition_node *condition = container_of(node, struct k_behavior_tree_condition_node, super);
    free(condition);
}

static struct k_behavior_tree_node *condition_create(struct k_behavior_tree *tree, void *data, enum k_behavior_tree_status (*fn_tick)(void *data)) {

    if (NULL == fn_tick)
        return NULL;

    struct k_behavior_tree_condition_node *condition = malloc(sizeof(struct k_behavior_tree_condition_node));
    if (NULL == condition)
        return NULL;

    condition->super.tree         = tree;
    condition->super.fn_interrupt = NULL;
    condition->super.fn_tick      = condition_tick;
    condition->super.fn_add_child = condition_add_child;
    condition->super.fn_destroy   = condition_destroy;

    condition->fn_tick = fn_tick;
    condition->data    = data;

    return &condition->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_condition(struct k_behavior_tree_node *node, void *data, enum k_behavior_tree_status (*fn_tick)(void *data)) {

    if (NULL == node || NULL == fn_tick)
        return NULL;

    struct k_behavior_tree_node *new_node = condition_create(node->tree, data, fn_tick);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add_child(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
