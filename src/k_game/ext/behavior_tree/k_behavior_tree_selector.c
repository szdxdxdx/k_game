#include <stdlib.h>

#include "k_array.h"

#include "./_internal.h"

struct k_behavior_tree_selector_node {

    struct k_behavior_tree_node super;

    struct k_array children;

    size_t index;
};

static int selector_add_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node) {
    struct k_behavior_tree_selector_node *seq = container_of(node, struct k_behavior_tree_selector_node, super);
    return k_array_push_back(&seq->children, &child_node);
}

static enum k_behavior_tree_status selector_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_selector_node *seq = (struct k_behavior_tree_selector_node *)node;

    struct k_array *array = &seq->children;
    if (seq->index == array->size) {
        if (0 == array->size)
            return K_BT_FAILURE;
        else
            seq->index = 0;
    }

    struct k_behavior_tree_node *child = k_array_get_elem(array, seq->index, struct k_behavior_tree_node *);

    enum k_behavior_tree_status result = child->fn_tick(child);
    switch (result) {
        case K_BT_RUNNING:
            return K_BT_RUNNING;

        case K_BT_SUCCESS:
            seq->index = array->size;
            return K_BT_SUCCESS;

        case K_BT_FAILURE:
            seq->index++;
            return (seq->index == array->size)
                ? K_BT_FAILURE
                : K_BT_RUNNING;
    }
}

static void selector_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_selector_node *seq = container_of(node, struct k_behavior_tree_selector_node, super);

    struct k_array *array = &seq->children;
    for (seq->index = 0; seq->index < array->size; seq->index++) {
        struct k_behavior_tree_node *child = k_array_get_elem(array, seq->index, struct k_behavior_tree_node *);
        child->fn_destroy(child);
    }

    k_array_destruct(&seq->children);
    free(seq);
}

struct k_behavior_tree_node *k_behavior_tree_add_selector(struct k_behavior_tree_node *node) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_selector_node *seq = malloc(sizeof(struct k_behavior_tree_selector_node));
    if (NULL == node)
        return NULL;

    seq->super.tree       = node->tree;
    seq->super.fn_add     = selector_add_child;
    seq->super.fn_tick    = selector_tick;
    seq->super.fn_destroy = selector_destroy;

    struct k_array_config config;
    config.fn_malloc     = malloc;
    config.fn_free       = free;
    config.elem_size     = sizeof(struct k_behavior_tree_node *);
    config.init_capacity = 4;
    if (NULL == k_array_construct(&seq->children, &config)) {
        free(node);
        return NULL;
    }

    seq->index = 0;

    if (0 != node->fn_add(node, &seq->super)) {
        k_array_destruct(&seq->children);
        selector_destroy(&seq->super);
        return NULL;
    }

    return &seq->super;
}
