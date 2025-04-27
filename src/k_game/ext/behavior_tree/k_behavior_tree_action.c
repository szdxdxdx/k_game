#include <assert.h>
#include <stdlib.h>

#include "./k_behavior_tree_internal.h"

struct k_behavior_tree_action_node {

    struct k_behavior_tree_node super;

    void *data;

    enum k_behavior_tree_status (*fn_tick)(void *data);

    void (*fn_interrupt)(void *data);
};

static enum k_behavior_tree_status action_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_action_node *action = container_of(node, struct k_behavior_tree_action_node, super);

    enum k_behavior_tree_status result = action->fn_tick(action->data);

    assert(result == K_BT_SUCCESS
        || result == K_BT_FAILURE
        || result == K_BT_RUNNING
    );

    return result;
}

static void action_interrupt_nop(struct k_behavior_tree_node *node) {
    (void)node;
}

static void action_interrupt(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_action_node *action = container_of(node, struct k_behavior_tree_action_node, super);

    if (NULL != action->fn_interrupt)
        action->fn_interrupt(action->data);
}

static int action_add_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child) {
    (void)node;
    (void)child;
    return -1;
}

static void action_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_action_node *action = container_of(node, struct k_behavior_tree_action_node, super);
    free(action);
}

static struct k_behavior_tree_node *action_create(struct k_behavior_tree *tree, void *data, enum k_behavior_tree_status (*fn_tick)(void *data), void (*fn_interrupt)(void *data)) {

    if (NULL == fn_tick)
        return NULL;

    struct k_behavior_tree_action_node *action = malloc(sizeof(struct k_behavior_tree_action_node));
    if (NULL == action)
        return NULL;

    action->super.tree         = tree;
    action->super.fn_tick      = action_tick;
    action->super.fn_interrupt = NULL != fn_interrupt ? action_interrupt : action_interrupt_nop;
    action->super.fn_add_child = action_add_child;
    action->super.fn_destroy   = action_destroy;

    action->data         = data;
    action->fn_tick      = fn_tick;
    action->fn_interrupt = fn_interrupt;

    return &action->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_action(struct k_behavior_tree_node *node, void *data, enum k_behavior_tree_status (*fn_tick)(void *data), void (*fn_interrupt)(void *data)) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_node *new_node = action_create(node->tree, data, fn_tick, fn_interrupt);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add_child(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
