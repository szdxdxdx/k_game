#include <stdlib.h>

#include "./_internal.h"

struct k_behavior_tree_action_node {

    struct k_behavior_tree_node super;

    enum k_behavior_tree_status (*fn_tick)(void *data);

    void *data;
};

static enum k_behavior_tree_status action_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_action_node *action = (struct k_behavior_tree_action_node *)node;
    return action->fn_tick(action->data);
}

static int action_add_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child) {
    (void)node;
    (void)child;
    return -1;
}

static void action_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_action_node *action = (struct k_behavior_tree_action_node *)node;
    free(action);
}

struct k_behavior_tree_node *k_behavior_tree_add_action(struct k_behavior_tree_node *node, void *data, enum k_behavior_tree_status (*fn_tick)(void *data)) {

    struct k_behavior_tree_action_node *action = malloc(sizeof(struct k_behavior_tree_action_node));
    if (NULL == action)
        return NULL;

    action->super.fn_tick    = action_tick;
    action->super.fn_add     = action_add_child;
    action->super.fn_destroy = action_destroy;
    action->super.tree       = node->tree;

    action->fn_tick = fn_tick;
    action->data    = data;

    if (0 != node->fn_add(node, &action->super))
        return NULL;

    return &action->super;
}
