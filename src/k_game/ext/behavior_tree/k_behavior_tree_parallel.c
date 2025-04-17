#include <stdlib.h>
#include <assert.h>

#include "k_array.h"

#include "./_internal.h"

struct k_behavior_tree_parallel_item {

    struct k_behavior_tree_node *child;

#define K__BT_PARALLEL_IDLE    0
#define K__BT_PARALLEL_RUNNING 1
#define K__BT_PARALLEL_SUCCESS 2
#define K__BT_PARALLEL_FAILURE 3
    int state;
};

struct k_behavior_tree_parallel_node {

    struct k_behavior_tree_node super;

    struct k_array children;

    int success_policy;

    int failure_policy;

    int running;
};

static void parallel_interrupt_(struct k_behavior_tree_parallel_node *parallel) {

    struct k_array *arr = &parallel->children;
    struct k_behavior_tree_parallel_item *items = arr->storage;
    size_t index = 0;
    size_t size  = arr->size;
    for (; index < size; index++) {

        if (items[index].state == K__BT_PARALLEL_RUNNING) {
            struct k_behavior_tree_node *child = items[index].child;

            child->fn_interrupt(child);
        }
    }

    parallel->running = 0;
}

static enum k_behavior_tree_status parallel_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_parallel_node *parallel = container_of(node, struct k_behavior_tree_parallel_node, super);

    struct k_array *arr = &parallel->children;
    struct k_behavior_tree_parallel_item *items = arr->storage;

    if ( ! parallel->running) {

        size_t size = arr->size;
        if (0 == size)
            return K_BT_SUCCESS;

        size_t index = 0;
        for (; index < size; index++) {
            items[index].state = K__BT_PARALLEL_IDLE;
        }

        parallel->running = 1;
    }

    int count_running = 0;
    int count_success = 0;
    int count_failure = 0;

    size_t size  = parallel->children.size;
    size_t index = 0;
    for (; index < size; index++) {

        if (items[index].state == K__BT_PARALLEL_SUCCESS) {
            count_success += 1;
            continue;
        }
        if (items[index].state == K__BT_PARALLEL_FAILURE) {
            count_failure += 1;
            continue;
        }

        struct k_behavior_tree_node *child = items[index].child;

        enum k_behavior_tree_status result = child->fn_tick(child);

        assert(result == K_BT_SUCCESS
            || result == K_BT_FAILURE
            || result == K_BT_RUNNING
        );

        switch (result) {

            case K_BT_RUNNING: {
                count_running += 1;
                items[index].state = K__BT_PARALLEL_RUNNING;
                continue;
            }

            case K_BT_SUCCESS: {
                count_success += 1;
                items[index].state = K__BT_PARALLEL_SUCCESS;

                if (count_success < parallel->success_policy)
                    continue;

                parallel_interrupt_(parallel);
                return K_BT_SUCCESS;
            }

            case K_BT_FAILURE: {
                count_failure += 1;
                items[index].state = K__BT_PARALLEL_FAILURE;

                if (count_failure < parallel->failure_policy)
                    continue;

                parallel_interrupt_(parallel);
                return K_BT_FAILURE;
            }
        }
    }

    if (0 == count_running) {
        parallel->running = 0;

        if (count_success == size) {
            return K_BT_SUCCESS;
        } else {
            return K_BT_FAILURE;
        }
    } else {
        return K_BT_RUNNING;
    }
}

static void parallel_interrupt(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_parallel_node *parallel = container_of(node, struct k_behavior_tree_parallel_node, super);

    if ( ! parallel->running)
        return;

    parallel_interrupt_(parallel);
}

static int parallel_add_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node) {
    struct k_behavior_tree_parallel_node *parallel = container_of(node, struct k_behavior_tree_parallel_node, super);

    struct k_array *arr = &parallel->children;
    struct k_behavior_tree_parallel_item *new_item = k_array_shift_right(arr, arr->size, 1);
    if (NULL == new_item)
        return -1;

    new_item->child = child_node;
    new_item->state = K__BT_PARALLEL_IDLE;
    return 0;
}

static void parallel_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_parallel_node *parallel = container_of(node, struct k_behavior_tree_parallel_node, super);

    struct k_array *arr = &parallel->children;
    struct k_behavior_tree_parallel_item *items = arr->storage;
    size_t index = 0;
    size_t size  = arr->size;
    for (; index < size; index++) {
        struct k_behavior_tree_node *child = items[index].child;

        child->fn_destroy(child);
    }

    k_array_destruct(&parallel->children);
    free(parallel);
}

static struct k_behavior_tree_node *parallel_create(struct k_behavior_tree *tree, int success_policy, int failure_policy) {

    if (success_policy <= 0 || failure_policy <= 0)
        return NULL;

    struct k_behavior_tree_parallel_node *parallel = malloc(sizeof(struct k_behavior_tree_parallel_node));
    if (NULL == parallel)
        return NULL;

    parallel->super.tree         = tree;
    parallel->super.fn_interrupt = parallel_interrupt;
    parallel->super.fn_tick      = parallel_tick;
    parallel->super.fn_add_child = parallel_add_child;
    parallel->super.fn_destroy   = parallel_destroy;

    if (NULL == k_array_construct(&parallel->children, sizeof(struct k_behavior_tree_parallel_item), NULL)) {
        free(parallel);
        return NULL;
    }

    parallel->success_policy = success_policy;
    parallel->failure_policy = failure_policy;

    parallel->running = 0;

    return &parallel->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_parallel(struct k_behavior_tree_node *node, int success_policy, int failure_policy) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_node *new_node = parallel_create(node->tree, success_policy, failure_policy);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add_child(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
