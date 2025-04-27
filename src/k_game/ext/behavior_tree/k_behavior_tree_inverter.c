#include <assert.h>
#include <stdlib.h>

#include "./k_behavior_tree_internal.h"

struct k_behavior_tree_inverter_node {

    struct k_behavior_tree_node super;

    struct k_behavior_tree_node *child;
};

static enum k_behavior_tree_status inverter_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_inverter_node *inverter = container_of(node, struct k_behavior_tree_inverter_node, super);

    struct k_behavior_tree_node *child = inverter->child;

    enum k_behavior_tree_status result = child->fn_tick(child);

    assert(result == K_BT_SUCCESS
        || result == K_BT_FAILURE
        || result == K_BT_RUNNING
    );

    switch (result) {
        case K_BT_RUNNING: return K_BT_RUNNING;
        case K_BT_SUCCESS: return K_BT_FAILURE;
        case K_BT_FAILURE: return K_BT_SUCCESS;
    }
}

static void inverter_interrupt(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_inverter_node *inverter = container_of(node, struct k_behavior_tree_inverter_node, super);

    struct k_behavior_tree_node *child = inverter->child;
    child->fn_interrupt(child);
}

static int inverter_set_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child) {
    struct k_behavior_tree_inverter_node *inverter = container_of(node, struct k_behavior_tree_inverter_node, super);

    if (&K__BEHAVIOR_TREE_DEFAULT_FAILURE == inverter->child) {
        inverter->child = child;
        return 0;
    } else {
        return -1;
    }
}

static void inverter_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_inverter_node *inverter = container_of(node, struct k_behavior_tree_inverter_node, super);

    struct k_behavior_tree_node *child = inverter->child;
    child->fn_destroy(child);

    free(inverter);
}

static struct k_behavior_tree_node *inverter_create(struct k_behavior_tree *tree) {

    struct k_behavior_tree_inverter_node *inverter = malloc(sizeof(struct k_behavior_tree_inverter_node));
    if (NULL == inverter)
        return NULL;

    inverter->super.tree         = tree;
    inverter->super.fn_tick      = inverter_tick;
    inverter->super.fn_interrupt = inverter_interrupt;
    inverter->super.fn_add_child = inverter_set_child;
    inverter->super.fn_destroy   = inverter_destroy;

    inverter->child = &K__BEHAVIOR_TREE_DEFAULT_FAILURE;

    return &inverter->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_inverter(struct k_behavior_tree_node *node) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_node *new_node = inverter_create(node->tree);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add_child(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
