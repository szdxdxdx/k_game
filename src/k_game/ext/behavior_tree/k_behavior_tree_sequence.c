#include <stdlib.h>

#include "k_array.h"

#include "./_internal.h"

struct k_behavior_tree_sequence_node {

    struct k_behavior_tree_node super;

    struct k_array children;

    size_t index;
};

static enum k_behavior_tree_status sequence_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_sequence_node *sequence = (struct k_behavior_tree_sequence_node *)node;

    struct k_array *array = &sequence->children;
    if (sequence->index == array->size) {
        if (0 == array->size)
            return K_BT_SUCCESS;
        else
            sequence->index = 0;
    }

    struct k_behavior_tree_node *child = k_array_get_elem(array, sequence->index, struct k_behavior_tree_node *);

    enum k_behavior_tree_status result = child->fn_tick(child);
    switch (result) {
        case K_BT_RUNNING:
            return K_BT_RUNNING;

        case K_BT_SUCCESS:
            sequence->index++;
            return (sequence->index == array->size)
                ? K_BT_SUCCESS
                : K_BT_RUNNING;

        case K_BT_FAILURE:
            sequence->index = array->size;
            return K_BT_FAILURE;
    }
}

static void sequence_interrupt(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_sequence_node *sequence = (struct k_behavior_tree_sequence_node *)node;

    struct k_array *array = &sequence->children;
    size_t index = 0;
    size_t size  = array->size;
    for (; index < size; index++) {
        struct k_behavior_tree_node *child = k_array_get_elem(array, index, struct k_behavior_tree_node *);

        if (NULL != child->fn_interrupt)
            child->fn_interrupt(child);
    }
}

static int sequence_add_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node) {
    struct k_behavior_tree_sequence_node *sequence = container_of(node, struct k_behavior_tree_sequence_node, super);
    return k_array_push_back(&sequence->children, &child_node);
}

static void sequence_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_sequence_node *sequence = container_of(node, struct k_behavior_tree_sequence_node, super);

    struct k_array *array = &sequence->children;
    size_t index = 0;
    size_t size  = array->size;
    for (; index < size; index++) {
        struct k_behavior_tree_node *child = k_array_get_elem(array, index, struct k_behavior_tree_node *);
        child->fn_destroy(child);
    }

    k_array_destruct(&sequence->children);
    free(sequence);
}

static struct k_behavior_tree_node *sequence_create(struct k_behavior_tree *tree) {

    struct k_behavior_tree_sequence_node *sequence = malloc(sizeof(struct k_behavior_tree_sequence_node));
    if (NULL == sequence)
        return NULL;

    sequence->super.tree         = tree;
    sequence->super.fn_tick      = sequence_tick;
    sequence->super.fn_interrupt = sequence_interrupt;
    sequence->super.fn_add_child = sequence_add_child;
    sequence->super.fn_destroy   = sequence_destroy;

    struct k_array_config config;
    config.fn_malloc     = malloc;
    config.fn_free       = free;
    config.elem_size     = sizeof(struct k_behavior_tree_node *);
    config.init_capacity = 4;
    if (NULL == k_array_construct(&sequence->children, &config)) {
        free(sequence);
        return NULL;
    }

    sequence->index = 0;

    return &sequence->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_sequence(struct k_behavior_tree_node *node) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_node *new_node = sequence_create(node->tree);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add_child(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
