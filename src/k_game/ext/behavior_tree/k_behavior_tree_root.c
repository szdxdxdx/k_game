#include <stdio.h>

#include "./_internal.h"

struct k_behavior_tree_root_node {

    struct k_behavior_tree_node super;

    struct k_behavior_tree_node *child;
};

struct k_behavior_tree {

    struct k_behavior_tree_root_node root;

    struct k_object *object;
};

static void k_behavior_tree_tick(struct k_object *object) {
    struct k_behavior_tree *tree = k_object_get_data(object);
    tree->root.child->fn_tick(tree->root.child);

    printf("~\n");
}

static int root_set_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node) {
    struct k_behavior_tree_root_node *root = container_of(node, struct k_behavior_tree_root_node, super);

    if (&K__BEHAVIOR_TREE_DEFAULT_SUCCESS == root->child) {
        root->child = child_node;
        return 0;
    } else {
        return -1;
    }
}

struct k_behavior_tree *k_behavior_tree_create(void) {

    struct k_object *object = k_object_create(sizeof(struct k_behavior_tree));
    if (NULL == object)
        return NULL;

    if (NULL == k_object_add_step_callback(object, k_behavior_tree_tick)) {
        k_object_destroy(object);
        return NULL;
    }

    struct k_behavior_tree *tree = k_object_get_data(object);
    tree->object = object;

    tree->root.super.tree         = tree;
    tree->root.super.fn_interrupt = NULL;
    tree->root.super.fn_tick      = NULL;
    tree->root.super.fn_add_child = root_set_child;
    tree->root.super.fn_destroy   = NULL;

    tree->root.child = &K__BEHAVIOR_TREE_DEFAULT_SUCCESS;

    return tree;
}

void k_behavior_tree_destroy(struct k_behavior_tree *tree) {

    if (NULL == tree)
        return;

    tree->root.child->fn_destroy(tree->root.child);

    k_object_destroy(tree->object);
}

struct k_behavior_tree_node *k_behavior_tree_get_root(struct k_behavior_tree *tree) {
    return &tree->root.super;
}
