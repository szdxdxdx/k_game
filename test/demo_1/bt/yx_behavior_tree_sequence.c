#include <assert.h>
#include "stdlib.h"

#include "./_internal.h"

static enum yx_behavior_tree_status sequence_tick(struct yx_behavior_tree_node *node) {
    struct yx_behavior_tree_sequence_node *seq = (struct yx_behavior_tree_sequence_node *)node;

    struct k_array *array = &seq->children;
    if (seq->index == array->size) {
        if (0 == array->size) {
            return YX_BT_SUCCESS;
        }
        else {
            seq->index = 0;
        }
    }

    struct yx_behavior_tree_node *child = k_array_get_elem(array, seq->index, struct yx_behavior_tree_node *);

    enum yx_behavior_tree_status result = child->fn_tick(child);

    if ( YX_BT_RUNNING == result) {
        return YX_BT_RUNNING;
    }
    else if (YX_BT_SUCCESS == result) {
        seq->index++;
        if (seq->index == array->size) {
            return YX_BT_SUCCESS;
        } else {
            return YX_BT_RUNNING;
        }
    }
    else if (YX_BT_FAILURE == result) {
        seq->index = array->size;
        return YX_BT_FAILURE;
    }
    else {
        assert(0);
        return YX_BT_FAILURE;
    }
}

static int sequence_add_child(struct yx_behavior_tree_node *node, struct yx_behavior_tree_node *child_node) {
    struct yx_behavior_tree_sequence_node *seq = container_of(node, struct yx_behavior_tree_sequence_node, super);
    return k_array_push_back(&seq->children, &child_node);
}

static void sequence_destroy(struct yx_behavior_tree_node *node) {
    struct yx_behavior_tree_sequence_node *seq = container_of(node, struct yx_behavior_tree_sequence_node, super);

    struct k_array *array = &seq->children;
    for (seq->index = 0; seq->index < array->size; seq->index++) {
        struct yx_behavior_tree_node *child = k_array_get_elem(array, seq->index, struct yx_behavior_tree_node *);
        child->fn_destroy(child);
    }

    k_array_destruct(&seq->children);
    free(seq);
}

struct yx_behavior_tree_node *yx_behavior_tree_add_sequence(struct yx_behavior_tree_node *node) {

    struct yx_behavior_tree_sequence_node *seq = malloc(sizeof(struct yx_behavior_tree_sequence_node));
    if (NULL == node)
        return NULL;

    seq->super.fn_tick    = sequence_tick;
    seq->super.fn_add     = sequence_add_child;
    seq->super.fn_destroy = sequence_destroy;
    seq->super.tree       = node->tree;

    struct k_array_config config;
    config.fn_malloc     = malloc;
    config.fn_free       = free;
    config.elem_size     = sizeof(struct yx_behavior_tree_node *);
    config.init_capacity = 4;
    if (NULL == k_array_construct(&seq->children, &config)) {
        free(node);
        return NULL;
    }

    seq->index = 0;

    if (0 != node->fn_add(node, &seq->super))
        sequence_destroy(&seq->super);

    return &seq->super;
}
