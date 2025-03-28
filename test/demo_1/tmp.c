#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/* region [behavior_tree] */

/* region [tree_node] */

enum k_BT_state {
    K_BT_FAILURE,
    K_BT_RUNNING,
    K_BT_SUCCESS
};

enum k_BT_node_type {
    K_BT_NODE_TYPE_ACTION,
};

struct k_BT_node {

    enum k_BT_node_type type;
};

/* region [action] */

struct k_BT_action_node {

    struct k_BT_node node_base;

    enum k_BT_state (*fn_tick)(void *data);

    void *data;
};

struct k_BT_node *k_BT_node_create_action(enum k_BT_state (*fn_tick)(void *data), void *data) {

    struct k_BT_action_node *node = malloc(sizeof(struct k_BT_action_node));
    node->node_base.type = K_BT_NODE_TYPE_ACTION;
    node->fn_tick = fn_tick;
    node->data = data;

    return (struct k_BT_node *)node;
}

enum k_BT_state k__BT_action_node_tick(struct k_BT_node *node) {
    struct k_BT_action_node *action_node = (struct k_BT_action_node *)node;

    return action_node->fn_tick(action_node->data);
}

/* endregion */

enum k_BT_state k__BT_node_tick(struct k_BT_node *node) {
    switch (node->type) {
        case K_BT_NODE_TYPE_ACTION: return k__BT_action_node_tick(node);
        default: assert(0);
    }
}

/* endregion */

/* region [tree] */

struct k_behavior_tree {

    struct k_BT_node *root;
};

void k_behavior_tree_run(struct k_behavior_tree *tree) {
    k__BT_node_tick(tree->root);
}

/* endregion */

/* endregion */

enum k_BT_state log_action(void *data) {
    printf("[log action] %s\n", (const char *)data);
    return K_BT_SUCCESS;
}

#if 1

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_BT_node *action_node = k_BT_node_create_action(log_action, "hello");

    struct k_behavior_tree *tree = malloc(sizeof(struct k_behavior_tree));
    tree->root = action_node;

    k_behavior_tree_run(tree);



    return 0;
}

#endif
