#include <stdlib.h>

#include "k_array.h"

#include "./_internal.h"

struct k_behavior_tree_selector_node {

    struct k_behavior_tree_node super;

    struct k_array children;

    size_t index;
};

static int selector_add_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node) {
    struct k_behavior_tree_selector_node *selector = container_of(node, struct k_behavior_tree_selector_node, super);
    return k_array_push_back(&selector->children, &child_node);
}

static enum k_behavior_tree_status selector_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_selector_node *selector = (struct k_behavior_tree_selector_node *)node;

    struct k_array *array = &selector->children;
    if (selector->index == array->size) {
        if (0 == array->size)
            return K_BT_FAILURE;
        else
            selector->index = 0;
    }

    struct k_behavior_tree_node *child = k_array_get_elem(array, selector->index, struct k_behavior_tree_node *);

    enum k_behavior_tree_status result = child->fn_tick(child);
    switch (result) {
        case K_BT_RUNNING:
            return K_BT_RUNNING;

        case K_BT_SUCCESS:
            selector->index = array->size;
            return K_BT_SUCCESS;

        case K_BT_FAILURE:
            selector->index++;
            return (selector->index == array->size)
                ? K_BT_FAILURE
                : K_BT_RUNNING;
    }
}

static void selector_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_selector_node *selector = container_of(node, struct k_behavior_tree_selector_node, super);

    struct k_array *array = &selector->children;
    for (selector->index = 0; selector->index < array->size; selector->index++) {
        struct k_behavior_tree_node *child = k_array_get_elem(array, selector->index, struct k_behavior_tree_node *);
        child->fn_destroy(child);
    }

    k_array_destruct(&selector->children);
    free(selector);
}

struct k_behavior_tree_node *k_behavior_tree_add_selector(struct k_behavior_tree_node *node) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_selector_node *selector = malloc(sizeof(struct k_behavior_tree_selector_node));
    if (NULL == node)
        return NULL;

    selector->super.tree       = node->tree;
    selector->super.fn_add     = selector_add_child;
    selector->super.fn_tick    = selector_tick;
    selector->super.fn_destroy = selector_destroy;

    struct k_array_config config;
    config.fn_malloc     = malloc;
    config.fn_free       = free;
    config.elem_size     = sizeof(struct k_behavior_tree_node *);
    config.init_capacity = 4;
    if (NULL == k_array_construct(&selector->children, &config)) {
        free(selector);
        return NULL;
    }

    selector->index = 0;

    if (0 != node->fn_add(node, &selector->super)) {
        k_array_destruct(&selector->children);
        free(selector);
        return NULL;
    }

    return &selector->super;
}
