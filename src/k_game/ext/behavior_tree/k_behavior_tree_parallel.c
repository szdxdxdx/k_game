#include <stdlib.h>
#include <assert.h>

#include "k_array.h"

#include "./_internal.h"

struct k_behavior_tree_parallel_node {

    struct k_behavior_tree_node super;

    struct k_array children;

    struct k_array status;

    int success_policy;

    int failure_policy;

    int running;
};

static enum k_behavior_tree_status parallel_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_parallel_node *parallel = container_of(node, struct k_behavior_tree_parallel_node, super);

    enum k_behavior_tree_status *status = k_array_get_elem_addr(&parallel->status, 0);
    struct k_behavior_tree_node **children = k_array_get_elem_addr(&parallel->children, 0);

    if ( ! parallel->running) {

        size_t size = parallel->status.size;
        if (0 == size)
            return K_BT_SUCCESS;

        size_t index = 0;
        for (; index < size; index++) {
            status[index] = K_BT_RUNNING;
        }

        parallel->running = 1;
    }

    int count_running = 0;
    int count_success = 0;
    int count_failure = 0;

    size_t size  = parallel->children.size;
    size_t index = 0;
    for (; index < size; index++) {

        switch (status[index]) {
            case K_BT_SUCCESS:
                count_success += 1;
                continue;
            case K_BT_FAILURE:
                count_failure += 1;
                continue;
            case K_BT_RUNNING:
                count_running += 1;
                break;
        }

        struct k_behavior_tree_node *child = children[index];

        enum k_behavior_tree_status result = child->fn_tick(child);

        assert(result == K_BT_SUCCESS
            || result == K_BT_FAILURE
            || result == K_BT_RUNNING
        );

        switch (result) {

            case K_BT_RUNNING: {
                count_running += 1;
                continue;
            }

            case K_BT_SUCCESS: {
                count_success += 1;
                status[index] = K_BT_SUCCESS;

                if (count_success < parallel->success_policy)
                    continue;

                for (index = 0; index < size; index++) {
                    if (status[index] == K_BT_RUNNING) {
                        child = children[index];
                        child->fn_interrupt(child);
                    }
                }
                parallel->running = 0;
                return K_BT_SUCCESS;
            }

            case K_BT_FAILURE: {
                count_failure += 1;
                status[index] = K_BT_FAILURE;

                if (count_failure < parallel->failure_policy)
                    continue;

                for (index = 0; index < size; index++) {
                    if (status[index] == K_BT_RUNNING) {
                        child = children[index];
                        child->fn_interrupt(child);
                    }
                }
                parallel->running = 0;
                return K_BT_FAILURE;
            }
        }
    }

    if (0 < count_running) {
        return K_BT_RUNNING;
    }
    else {
        parallel->running = 0;

        if (count_success == size) {
            return K_BT_SUCCESS;
        } else {
            return K_BT_FAILURE;
        }
    }
}

static void parallel_interrupt(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_parallel_node *parallel = container_of(node, struct k_behavior_tree_parallel_node, super);

    if ( ! parallel->running)
        return;

    struct k_behavior_tree_node **children = k_array_get_elem_addr(&parallel->children, 0);
    enum k_behavior_tree_status *status    = k_array_get_elem_addr(&parallel->status, 0);
    size_t size  = parallel->children.size;
    size_t index = 0;
    for (; index < size; index++) {

        if (status[index] == K_BT_RUNNING) {
            struct k_behavior_tree_node *child = children[index];

            child->fn_interrupt(child);
        }
    }

    parallel->running = 0;
}

static int parallel_add_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node) {
    struct k_behavior_tree_parallel_node *parallel = container_of(node, struct k_behavior_tree_parallel_node, super);

    if (0 != k_array_push_back(&parallel->children, &child_node)) {
        return -1;
    }

    if (NULL == k_array_shift_right(&parallel->status, parallel->status.size, 1)) {
        k_array_pop_back(&parallel->children);
        return -1;
    }

    return 0;
}

static void parallel_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_parallel_node *parallel = container_of(node, struct k_behavior_tree_parallel_node, super);

    struct k_array *array = &parallel->children;
    size_t index = 0;
    size_t size  = array->size;
    for (; index < size; index++) {
        struct k_behavior_tree_node *child = k_array_get_elem(array, index, struct k_behavior_tree_node *);
        child->fn_destroy(child);
    }

    k_array_destruct(&parallel->children);
    k_array_destruct(&parallel->status);
    free(parallel);
}

static struct k_behavior_tree_node *parallel_create(struct k_behavior_tree *tree, int success_policy, int failure_policy) {

    struct k_behavior_tree_parallel_node *parallel = malloc(sizeof(struct k_behavior_tree_parallel_node));
    if (NULL == parallel)
        return NULL;

    parallel->super.tree         = tree;
    parallel->super.fn_interrupt = parallel_interrupt;
    parallel->super.fn_tick      = parallel_tick;
    parallel->super.fn_add_child = parallel_add_child;
    parallel->super.fn_destroy   = parallel_destroy;

    struct k_array_config config;
    config.fn_malloc     = malloc;
    config.fn_free       = free;
    config.elem_size     = sizeof(struct k_behavior_tree_node *);
    config.init_capacity = 4;
    if (NULL == k_array_construct(&parallel->children, &config)) {
        free(parallel);
        return NULL;
    }

    config.elem_size = sizeof(enum k_behavior_tree_status);
    if (NULL == k_array_construct(&parallel->status, &config)) {
        k_array_destruct(&parallel->children);
        free(parallel);
        return NULL;
    }

    parallel->success_policy = success_policy;
    parallel->failure_policy = failure_policy;

    parallel->running = 0;

    return &parallel->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_parallel(struct k_behavior_tree_node *node) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_node *new_node = parallel_create(node->tree, INT_MAX, 1);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add_child(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
