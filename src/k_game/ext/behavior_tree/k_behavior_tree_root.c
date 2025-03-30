#include "./_internal.h"

struct k_behavior_tree_root_node {

    struct k_behavior_tree_node super;

    struct k_behavior_tree_node *child;
};

struct k_behavior_tree {

    struct k_behavior_tree_root_node root;

    struct k_object *object;
};

static enum k_behavior_tree_status do_nothing_tick(struct k_behavior_tree_node *node) {
    (void)node;
    return K_BT_SUCCESS;
}

static struct k_behavior_tree_node NULL_CHILD = {
    .fn_add     = NULL,
    .fn_tick    = do_nothing_tick,
    .fn_destroy = NULL,
};

static int root_set_child(struct k_behavior_tree_node *node, struct k_behavior_tree_node *child_node) {
    struct k_behavior_tree_root_node *root = container_of(node, struct k_behavior_tree_root_node, super);

    if (&NULL_CHILD == root->child) {
        root->child = child_node;
        return 0;
    } else {
        return -1;
    }
}

static void k_behavior_tree_tick(struct k_object *object) {
    struct k_behavior_tree *tree = k_object_get_data(object);

    tree->root.child->fn_tick(tree->root.child);
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

    tree->root.super.tree       = tree;
    tree->root.super.fn_add     = root_set_child;
    tree->root.super.fn_tick    = NULL;
    tree->root.super.fn_destroy = NULL;

    tree->root.child = &NULL_CHILD;

    return tree;
}

void k_behavior_tree_destroy(struct k_behavior_tree *tree) {

    if (NULL == tree)
        return;

    struct k_behavior_tree_node *child = tree->root.child;
    if (&NULL_CHILD != child) {
        child->fn_destroy(child);
    }

    k_object_destroy(tree->object);
}

struct k_behavior_tree_node *k_behavior_tree_get_root(struct k_behavior_tree *tree) {
    return &tree->root.super;
}
