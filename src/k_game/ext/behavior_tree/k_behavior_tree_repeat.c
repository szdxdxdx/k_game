#include <stdlib.h>

#include "./_internal.h"

struct k_behavior_tree_repeat_node {

    struct k_behavior_tree_node super;

    struct k_behavior_tree_node *child;

    size_t count;

    size_t n;

    int running;
};

static void repeat_interrupt_(struct k_behavior_tree_repeat_node *repeat) {

    struct k_behavior_tree_node *child = repeat->child;
    child->fn_interrupt(child);

    repeat->running = 0;
}

static enum k_behavior_tree_status repeat_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_repeat_node *repeat = container_of(node, struct k_behavior_tree_repeat_node, super);

    if ( ! repeat->running) {
        repeat->count = 0;
        repeat->running = 1;
    }

    struct k_behavior_tree_node *child = repeat->child;

    while (repeat->count < repeat->n) {

        enum k_behavior_tree_status result = child->fn_tick(child);
        switch (result) {
            case K_BT_SUCCESS:
                repeat->count++;
                break;
            case K_BT_FAILURE:
                repeat_interrupt_(repeat);
                return K_BT_FAILURE;
            case K_BT_RUNNING:
                return K_BT_RUNNING;
        }
    }

    repeat->running = 0;
    return K_BT_SUCCESS;
}

static void repeat_interrupt(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_repeat_node *repeat = container_of(node, struct k_behavior_tree_repeat_node, super);

    if (repeat->running) {
        repeat_interrupt_(repeat);
    }
}

static int repeat_set_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child) {
    struct k_behavior_tree_repeat_node *repeat = container_of(node, struct k_behavior_tree_repeat_node, super);

    if (&K__BEHAVIOR_TREE_DEFAULT_SUCCESS == repeat->child) {
        repeat->child = child;
        return 0;
    } else {
        return -1;
    }
}

static void repeat_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_repeat_node *repeat = container_of(node, struct k_behavior_tree_repeat_node, super);

    struct k_behavior_tree_node *child = repeat->child;
    child->fn_destroy(child);

    free(repeat);
}

static struct k_behavior_tree_node *repeat_create(struct k_behavior_tree *tree, size_t n) {

    if (n == 0)
        return NULL;

    struct k_behavior_tree_repeat_node *repeat = malloc(sizeof(struct k_behavior_tree_repeat_node));
    if (NULL == repeat)
        return NULL;

    repeat->super.tree         = tree;
    repeat->super.fn_tick      = repeat_tick;
    repeat->super.fn_interrupt = repeat_interrupt;
    repeat->super.fn_add_child = repeat_set_child;
    repeat->super.fn_destroy   = repeat_destroy;

    repeat->child = &K__BEHAVIOR_TREE_DEFAULT_SUCCESS;

    repeat->count = 0;
    repeat->n     = n;

    repeat->running = 0;

    return &repeat->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_repeat(struct k_behavior_tree_node *node, size_t n) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_node *new_node = repeat_create(node->tree, n);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add_child(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
