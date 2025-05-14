#include <assert.h>
#include <stdlib.h>

#include "./k_behavior_tree_internal.h"

struct k_behavior_tree_force_failure_node {

    struct k_behavior_tree_node super;

    struct k_behavior_tree_node *child;
};

static enum k_behavior_tree_status force_failure_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_force_failure_node *force_failure = container_of(node, struct k_behavior_tree_force_failure_node, super);

    struct k_behavior_tree_node *child = force_failure->child;

    enum k_behavior_tree_status result = child->fn_tick(child);

    assert(result == K_BT_SUCCESS
        || result == K_BT_FAILURE
        || result == K_BT_RUNNING
    );

    if (K_BT_RUNNING == result) {
        return K_BT_RUNNING;
    } else {
        return K_BT_FAILURE;
    }
}

static void force_failure_interrupt(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_force_failure_node *force_failure = container_of(node, struct k_behavior_tree_force_failure_node, super);

    struct k_behavior_tree_node *child = force_failure->child;
    child->fn_interrupt(child);
}

static void force_failure_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_force_failure_node *force_failure = container_of(node, struct k_behavior_tree_force_failure_node, super);

    struct k_behavior_tree_node *child = force_failure->child;
    child->fn_destroy(child);

    free(force_failure);
}

static int force_failure_set_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child) {
    struct k_behavior_tree_force_failure_node *force_failure = container_of(node, struct k_behavior_tree_force_failure_node, super);

    if (&K__BEHAVIOR_TREE_DEFAULT_FAILURE == force_failure->child) {
        force_failure->child = child;
        return 0;
    } else {
        return -1;
    }
}

static struct k_behavior_tree_node *force_failure_create(struct k_behavior_tree *tree) {

    struct k_behavior_tree_force_failure_node *force_failure = malloc(sizeof(struct k_behavior_tree_force_failure_node));
    if (NULL == force_failure)
        return NULL;

    force_failure->super.tree         = tree;
    force_failure->super.fn_tick      = force_failure_tick;
    force_failure->super.fn_interrupt = force_failure_interrupt;
    force_failure->super.fn_add_child = force_failure_set_child;
    force_failure->super.fn_destroy   = force_failure_destroy;

    force_failure->child = &K__BEHAVIOR_TREE_DEFAULT_FAILURE;

    return &force_failure->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_force_failure(struct k_behavior_tree_node *node) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_node *new_node = force_failure_create(node->tree);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add_child(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
