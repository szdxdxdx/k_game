#include <assert.h>
#include <stdlib.h>

#include "./_internal.h"

struct k_behavior_tree_root_node {

    struct k_behavior_tree_node super;

    struct k_behavior_tree_node *child;
};

struct k_behavior_tree {

    struct k_behavior_tree_root_node root;
};

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

    struct k_behavior_tree *tree = malloc(sizeof(struct k_behavior_tree));
    if (NULL == tree)
        return NULL;

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
}

void k_behavior_tree_tick(struct k_behavior_tree *tree) {

    struct k_behavior_tree_node *child = tree->root.child;

    enum k_behavior_tree_status result = child->fn_tick(child);

    assert(result == K_BT_SUCCESS
        || result == K_BT_FAILURE
        || result == K_BT_RUNNING
    );
}

struct k_behavior_tree_node *k_behavior_tree_get_root(struct k_behavior_tree *tree) {
    return &tree->root.super;
}
