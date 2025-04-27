#include <assert.h>
#include <stdlib.h>

#include "k_array.h"

#include "./k_behavior_tree_internal.h"

struct k_behavior_tree_sequence_node {

    struct k_behavior_tree_node super;

    struct k_array children;

    size_t index;

    int running;
};

static enum k_behavior_tree_status sequence_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_sequence_node *sequence = container_of(node, struct k_behavior_tree_sequence_node, super);

    struct k_array *arr = &sequence->children;
    if ( ! sequence->running) {
        if (0 == arr->size)
            return K_BT_SUCCESS;
        else {
            sequence->index   = 0;
            sequence->running = 1;
        }
    }

    struct k_behavior_tree_node *child;
    size_t size = arr->size;
    for (; sequence->index < size; sequence->index++) {
        child = k_array_get_elem(arr, sequence->index, struct k_behavior_tree_node *);

        enum k_behavior_tree_status result = child->fn_tick(child);

        assert(result == K_BT_SUCCESS
            || result == K_BT_FAILURE
            || result == K_BT_RUNNING
        );

        switch (result) {
            case K_BT_RUNNING:
                return K_BT_RUNNING;

            case K_BT_SUCCESS:
                continue;

            case K_BT_FAILURE:
                sequence->running = 0;
                return K_BT_FAILURE;
        }
    }

    sequence->running = 0;
    return K_BT_SUCCESS;
}

static void sequence_interrupt(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_sequence_node *sequence = container_of(node, struct k_behavior_tree_sequence_node, super);

    if ( ! sequence->running)
        return;

    struct k_array *arr = &sequence->children;
    size_t index = sequence->index;
    size_t size  = arr->size;
    for (; index < size; index++) {
        struct k_behavior_tree_node *child = k_array_get_elem(arr, index, struct k_behavior_tree_node *);

        child->fn_interrupt(child);
    }

    sequence->running = 0;
}

static int sequence_add_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node) {
    struct k_behavior_tree_sequence_node *sequence = container_of(node, struct k_behavior_tree_sequence_node, super);
    return k_array_push_back(&sequence->children, &child_node);
}

static void sequence_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_sequence_node *sequence = container_of(node, struct k_behavior_tree_sequence_node, super);

    struct k_array *arr = &sequence->children;
    size_t index = 0;
    size_t size  = arr->size;
    for (; index < size; index++) {
        struct k_behavior_tree_node *child = k_array_get_elem(arr, index, struct k_behavior_tree_node *);
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

    if (NULL == k_array_construct(&sequence->children, sizeof(struct k_behavior_tree_node *), NULL)) {
        free(sequence);
        return NULL;
    }

    sequence->index   = 0;
    sequence->running = 0;

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
