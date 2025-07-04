#include <assert.h>
#include <stdlib.h>

#include "k_array.h"

#include "./k_behavior_tree_internal.h"

struct k_behavior_tree_selector_node {

    struct k_behavior_tree_node super;

    struct k_array children;

    size_t index;

    int running;
};

static enum k_behavior_tree_status selector_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_selector_node *selector = (struct k_behavior_tree_selector_node *)node;

    struct k_array *arr = &selector->children;
    if ( ! selector->running) {
        if (0 == arr->size) {
            return K_BT_FAILURE;
        }
        else {
            selector->index   = 0;
            selector->running = 1;
        }
    }

    struct k_behavior_tree_node *child;
    size_t size = arr->size;
    for (; selector->index < size; selector->index++) {
        child = k_array_get_elem(arr, selector->index, struct k_behavior_tree_node *);

        enum k_behavior_tree_status result = child->fn_tick(child);

        assert(result == K_BT_SUCCESS
            || result == K_BT_FAILURE
            || result == K_BT_RUNNING
        );

        switch (result) {
            case K_BT_RUNNING:
                return K_BT_RUNNING;

            case K_BT_SUCCESS:
                selector->running = 0;
                return K_BT_SUCCESS;

            case K_BT_FAILURE:
                continue;
        }
    }

    selector->running = 0;
    return K_BT_FAILURE;
}

static void selector_interrupt(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_selector_node *selector = container_of(node, struct k_behavior_tree_selector_node, super);

    if ( ! selector->running)
        return;

    struct k_array *arr = &selector->children;
    size_t index = selector->index;
    size_t size  = arr->size;
    for (; index < size; index++) {
        struct k_behavior_tree_node *child = k_array_get_elem(arr, index, struct k_behavior_tree_node *);

        child->fn_interrupt(child);
    }

    selector->running = 0;
}

static int selector_add_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node) {
    struct k_behavior_tree_selector_node *selector = container_of(node, struct k_behavior_tree_selector_node, super);
    return k_array_push_back(&selector->children, &child_node);
}

static void selector_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_selector_node *selector = container_of(node, struct k_behavior_tree_selector_node, super);

    struct k_array *arr = &selector->children;
    size_t index = 0;
    size_t size  = arr->size;
    for (; index < size; index++) {
        struct k_behavior_tree_node *child = k_array_get_elem(arr, index, struct k_behavior_tree_node *);
        child->fn_destroy(child);
    }

    k_array_destruct(&selector->children);
    free(selector);
}

static struct k_behavior_tree_node *selector_create(struct k_behavior_tree *tree) {

    struct k_behavior_tree_selector_node *selector = malloc(sizeof(struct k_behavior_tree_selector_node));
    if (NULL == selector)
        return NULL;

    selector->super.tree         = tree;
    selector->super.fn_tick      = selector_tick;
    selector->super.fn_interrupt = selector_interrupt;
    selector->super.fn_add_child = selector_add_child;
    selector->super.fn_destroy   = selector_destroy;

    if (NULL == k_array_construct(&selector->children, sizeof(struct k_behavior_tree_node *), NULL)) {
        free(selector);
        return NULL;
    }

    selector->index   = 0;
    selector->running = 0;

    return &selector->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_selector(struct k_behavior_tree_node *node) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_node *new_node = selector_create(node->tree);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add_child(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
