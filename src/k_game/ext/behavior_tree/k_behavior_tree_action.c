#include <stdlib.h>

#include "./_internal.h"

struct k_behavior_tree_action_node {

    struct k_behavior_tree_node super;

    enum k_behavior_tree_status (*fn_tick)(void *data);

    void *data;
};

static int action_add_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child) {
    (void)node;
    (void)child;
    return -1;
}

static enum k_behavior_tree_status action_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_action_node *action = (struct k_behavior_tree_action_node *)node;
    return action->fn_tick(action->data);
}

static void action_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_action_node *action = (struct k_behavior_tree_action_node *)node;
    free(action);
}

static struct k_behavior_tree_node *action_create(struct k_behavior_tree *tree, void *data, enum k_behavior_tree_status (*fn_tick)(void *data)) {

    if (NULL == fn_tick)
        return NULL;

    struct k_behavior_tree_action_node *action = malloc(sizeof(struct k_behavior_tree_action_node));
    if (NULL == action)
        return NULL;

    action->super.tree       = tree;
    action->super.fn_add     = action_add_child;
    action->super.fn_tick    = action_tick;
    action->super.fn_destroy = action_destroy;

    action->fn_tick = fn_tick;
    action->data    = data;

    return &action->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_action(struct k_behavior_tree_node *node, void *data, enum k_behavior_tree_status (*fn_tick)(void *data)) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_node *new_node = action_create(node->tree, data, fn_tick);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
