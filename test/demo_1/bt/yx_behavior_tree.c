#include <stdio.h>

#include "./_internal.h"

static enum yx_behavior_tree_status do_nothing_tick(struct yx_behavior_tree_node *node) {
    (void)node;
    return YX_BT_SUCCESS;
}

static struct yx_behavior_tree_node NULL_CHILD = {
    .fn_add     = NULL,
    .fn_tick    = do_nothing_tick,
    .fn_destroy = NULL,
};

static int root_set_child(struct yx_behavior_tree_node *node, struct yx_behavior_tree_node *child_node) {
    struct yx_behavior_tree_root_node *root = container_of(node, struct yx_behavior_tree_root_node, super);

    if (&NULL_CHILD == root->child) {
        root->child = child_node;
        return 0;
    } else {
        return -1;
    }
}

static void yx_behavior_tree_tick(struct k_object *b) {
    struct yx_behavior_tree *tree = k_object_get_data(b);

    struct yx_behavior_tree_node *child = tree->root.child;

    child->fn_tick(child);

    printf("=============\n");
}

struct yx_behavior_tree *yx_behavior_tree_create(void) {

    struct k_object *object = k_object_create(sizeof(struct yx_behavior_tree));
    if (NULL == object)
        return NULL;

    if (NULL == k_object_add_step_callback(object, yx_behavior_tree_tick)) {
        k_object_destroy(object);
        return NULL;
    }

    struct yx_behavior_tree *tree = k_object_get_data(object);

    tree->root.super.fn_add     = root_set_child;
    tree->root.super.fn_tick    = NULL;
    tree->root.super.fn_destroy = NULL;
    tree->root.super.tree       = tree;
    tree->root.child = &NULL_CHILD;

    tree->object = object;

    return tree;
}

void yx_behavior_tree_destroy(struct yx_behavior_tree *tree) {

    if (NULL == tree)
        return;

    struct yx_behavior_tree_node *child = tree->root.child;
    if (&NULL_CHILD != child) {
        child->fn_destroy(child);
    }

    k_object_destroy(tree->object);
}

struct yx_behavior_tree_node *yx_behavior_tree_get_root(struct yx_behavior_tree *tree) {
    return &tree->root.super;
}
