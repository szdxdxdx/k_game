#include <assert.h>
#include <stdlib.h>

#include "./_internal.h"

struct k_behavior_tree_force_success_node {

    struct k_behavior_tree_node super;

    struct k_behavior_tree_node *child;
};

static enum k_behavior_tree_status force_success_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_force_success_node *force_success = container_of(node, struct k_behavior_tree_force_success_node, super);

    struct k_behavior_tree_node *child = force_success->child;

    enum k_behavior_tree_status result = child->fn_tick(child);

    assert(result == K_BT_SUCCESS
        || result == K_BT_FAILURE
        || result == K_BT_RUNNING
    );

    if (K_BT_RUNNING == result) {
        return K_BT_RUNNING;
    } else {
        return K_BT_SUCCESS;
    }
}

static void force_success_interrupt(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_force_success_node *force_success = container_of(node, struct k_behavior_tree_force_success_node, super);

    struct k_behavior_tree_node *child = force_success->child;
    child->fn_interrupt(child);
}

static int force_success_set_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child) {
    struct k_behavior_tree_force_success_node *force_success = container_of(node, struct k_behavior_tree_force_success_node, super);

    if (&K__BEHAVIOR_TREE_DEFAULT_SUCCESS == force_success->child) {
        force_success->child = child;
        return 0;
    } else {
        return -1;
    }
}

static void force_success_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_force_success_node *force_success = container_of(node, struct k_behavior_tree_force_success_node, super);

    struct k_behavior_tree_node *child = force_success->child;
    child->fn_destroy(child);

    free(force_success);
}

static struct k_behavior_tree_node *force_success_create(struct k_behavior_tree *tree) {

    struct k_behavior_tree_force_success_node *force_success = malloc(sizeof(struct k_behavior_tree_force_success_node));
    if (NULL == force_success)
        return NULL;

    force_success->super.tree         = tree;
    force_success->super.fn_tick      = force_success_tick;
    force_success->super.fn_interrupt = force_success_interrupt;
    force_success->super.fn_add_child = force_success_set_child;
    force_success->super.fn_destroy   = force_success_destroy;

    force_success->child = &K__BEHAVIOR_TREE_DEFAULT_SUCCESS;

    return &force_success->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_force_success(struct k_behavior_tree_node *node) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_node *new_node = force_success_create(node->tree);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add_child(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
