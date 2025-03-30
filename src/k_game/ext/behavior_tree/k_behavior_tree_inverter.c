#include <stdlib.h>

#include "./_internal.h"

struct k_behavior_tree_inverter_node {

    struct k_behavior_tree_node super;

    struct k_behavior_tree_node *child;
};

static int inverter_set_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child) {
    struct k_behavior_tree_inverter_node *inv = (struct k_behavior_tree_inverter_node *)node;

    if (NULL == inv->child) {
        inv->child = child;
        return 0;
    } else {
        return -1;
    }
}

static enum k_behavior_tree_status inverter_tick(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_inverter_node *inv = (struct k_behavior_tree_inverter_node *)node;

    struct k_behavior_tree_node *child = inv->child;

    enum k_behavior_tree_status result = child->fn_tick(child);
    switch (result) {
        case K_BT_RUNNING: return K_BT_RUNNING;
        case K_BT_SUCCESS: return K_BT_FAILURE;
        case K_BT_FAILURE: return K_BT_SUCCESS;
    }
}

static void inverter_destroy(struct k_behavior_tree_node *node) {
    struct k_behavior_tree_inverter_node *inv = (struct k_behavior_tree_inverter_node *)node;
    free(inv);
}

struct k_behavior_tree_node *k_behavior_tree_add_inverter(struct k_behavior_tree_node *node) {

    if (NULL == node)
        return NULL;

    struct k_behavior_tree_inverter_node *inv = malloc(sizeof(struct k_behavior_tree_inverter_node));
    if (NULL == inv)
        return NULL;

    inv->super.tree       = node->tree;
    inv->super.fn_add     = inverter_set_child;
    inv->super.fn_tick    = inverter_tick;
    inv->super.fn_destroy = inverter_destroy;

    if (0 != node->fn_add(node, &inv->super))
        return NULL;

    return &inv->super;
}
