#include <assert.h>
#include <stdlib.h>

#include "k_game.h"

#include "./k_behavior_tree_internal.h"

struct k_behavior_tree_delay_node {

    struct k_behavior_tree_node super;

    struct k_behavior_tree_node *child;

    uint64_t delay_ms;

    uint64_t timeout;

#define K__BT_DELAY_IDLE    0
#define K__BT_DELAY_WAITING 1
#define K__BT_DELAY_RUNNING 2
    int state;
};

static enum k_behavior_tree_status delay_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_delay_node *delay = container_of(node, struct k_behavior_tree_delay_node, super);

    uint64_t timestamp = node->tree->timestamp;

    if (K__BT_DELAY_IDLE == delay->state) {
        delay->timeout = delay->delay_ms + timestamp;
        delay->state = K__BT_DELAY_WAITING;
        return K_BT_RUNNING;
    }

    if (K__BT_DELAY_WAITING == delay->state) {

        if (timestamp < delay->timeout) {
            return K_BT_RUNNING;
        }

        delay->state = K__BT_DELAY_RUNNING;
    }

    struct k_behavior_tree_node *child = delay->child;

    enum k_behavior_tree_status result = child->fn_tick(child);

    assert(result == K_BT_SUCCESS
        || result == K_BT_FAILURE
        || result == K_BT_RUNNING
    );

    if (result == K_BT_RUNNING) {
        return K_BT_RUNNING;
    } else {
        delay->state = K__BT_DELAY_IDLE;
        return result;
    }
}

static void delay_interrupt(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_delay_node *delay = container_of(node, struct k_behavior_tree_delay_node, super);

    if (K__BT_DELAY_RUNNING != delay->state)
        return;

    struct k_behavior_tree_node *child = delay->child;
    child->fn_interrupt(child);

    delay->state = K__BT_DELAY_IDLE;
}

static int delay_set_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child) {
    struct k_behavior_tree_delay_node *delay = container_of(node, struct k_behavior_tree_delay_node, super);

    if (&K__BEHAVIOR_TREE_DEFAULT_SUCCESS == delay->child) {
        delay->child = child;
        return 0;
    } else {
        return -1;
    }
}

static void delay_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_delay_node *delay = container_of(node, struct k_behavior_tree_delay_node, super);

    struct k_behavior_tree_node *child = delay->child;
    child->fn_destroy(child);

    free(delay);
}

static struct k_behavior_tree_node *delay_create(struct k_behavior_tree *tree, int delay_ms) {

    if (delay_ms <= 0)
        return NULL;

    struct k_behavior_tree_delay_node *delay = malloc(sizeof(struct k_behavior_tree_delay_node));
    if (NULL == delay)
        return NULL;

    delay->super.tree         = tree;
    delay->super.fn_tick      = delay_tick;
    delay->super.fn_interrupt = delay_interrupt;
    delay->super.fn_add_child = delay_set_child;
    delay->super.fn_destroy   = delay_destroy;

    delay->child = &K__BEHAVIOR_TREE_DEFAULT_SUCCESS;

    delay->delay_ms = (uint64_t)delay_ms;
    delay->timeout  = 0;
    delay->state    = K__BT_DELAY_IDLE;

    return &delay->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_delay(struct k_behavior_tree_node *node, int delay_ms) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_node *new_node = delay_create(node->tree, delay_ms);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add_child(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
