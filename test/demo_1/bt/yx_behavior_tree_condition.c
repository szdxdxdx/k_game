#include <malloc.h>

#include "./_internal.h"

static enum yx_behavior_tree_status condition_tick(struct yx_behavior_tree_node *node) {
    struct yx_behavior_tree_condition_node *condition = (struct yx_behavior_tree_condition_node *)node;

    return condition->fn_tick(condition->data);
}

static int condition_add_child(struct yx_behavior_tree_node *node, struct yx_behavior_tree_node *child) {
    (void)node;
    (void)child;
    return -1;
}

static void condition_destroy(struct yx_behavior_tree_node *node) {
    struct yx_behavior_tree_condition_node *condition = (struct yx_behavior_tree_condition_node *)node;
    free(condition);
}

struct yx_behavior_tree_node *yx_behavior_tree_add_condition(struct yx_behavior_tree_node *node, void *data, enum yx_behavior_tree_status (*fn_tick)(void *data)) {

    struct yx_behavior_tree_condition_node *condition = malloc(sizeof(struct yx_behavior_tree_condition_node));
    if (NULL == condition)
        return NULL;

    condition->super.fn_tick    = condition_tick;
    condition->super.fn_add     = condition_add_child;
    condition->super.fn_destroy = condition_destroy;
    condition->super.tree       = node->tree;

    condition->fn_tick = fn_tick;
    condition->data    = data;

    if (0 != node->fn_add(node, &condition->super))
        return NULL;

    return &condition->super;
}
