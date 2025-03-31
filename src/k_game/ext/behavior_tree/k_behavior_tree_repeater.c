#include <stdlib.h>

#include "./_internal.h"

struct k_behavior_tree_repeater_node {

    struct k_behavior_tree_node super;

    struct k_behavior_tree_node *child;

    size_t count;
    size_t n;
};

static enum k_behavior_tree_status repeater_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_repeater_node *repeater = container_of(node, struct k_behavior_tree_repeater_node, super);

}

static void repeater_interrupt(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_repeater_node *repeater = container_of(node, struct k_behavior_tree_repeater_node, super);

    /* TODO */

    struct k_behavior_tree_node *child = repeater->child;
    child->fn_interrupt(child);
}

static int repeater_set_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child) {
    struct k_behavior_tree_repeater_node *repeater = container_of(node, struct k_behavior_tree_repeater_node, super);

}

static void repeater_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_repeater_node *repeater = container_of(node, struct k_behavior_tree_repeater_node, super);

    struct k_behavior_tree_node *child = repeater->child;
    child->fn_destroy(child);

    free(repeater);
}

static struct k_behavior_tree_node *repeater_create(struct k_behavior_tree *tree, size_t n) {

    struct k_behavior_tree_repeater_node *repeater = malloc(sizeof(struct k_behavior_tree_repeater_node));
    if (NULL == repeater)
        return NULL;

    repeater->super.tree         = tree;
    repeater->super.fn_tick      = repeater_tick;
    repeater->super.fn_interrupt = repeater_interrupt;
    repeater->super.fn_add_child = repeater_set_child;
    repeater->super.fn_destroy   = repeater_destroy;

    repeater->child = &K__BEHAVIOR_TREE_DEFAULT_SUCCESS;

    return &repeater->super;
}

struct k_behavior_tree_node *k_behavior_tree_add_repeater(struct k_behavior_tree_node *node, size_t n) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_node *new_node = repeater_create(node->tree, n);
    if (NULL == new_node)
        return NULL;

    if (0 != node->fn_add_child(node, new_node)) {
        new_node->fn_destroy(new_node);
        return NULL;
    }

    return new_node;
}
