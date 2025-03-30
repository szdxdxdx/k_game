#include <stdio.h>

#include "k_game.h"

static enum k_behavior_tree_status action_count(void *data) {

    int *count = data;
    *count += 1;
    printf("count++\n");
    return K_BT_SUCCESS;
}

static enum k_behavior_tree_status condition_count(void *data) {

    int *count = data;
    if (*count > 3) {
        *count = 0;
        printf("condition = true, set count = 0\n");
        return K_BT_SUCCESS;
    }
    else {
        printf("condition = false\n");
        return K_BT_FAILURE;
    }
}

static enum k_behavior_tree_status action_log_count(void *data) {

    printf("count = %d\n", *(int *)data);

    return K_BT_SUCCESS;
}

static enum k_behavior_tree_status action_log(void *data) {

    printf("%s\n", (const char *)data);

    return K_BT_SUCCESS;
}

static int count = 0;

void yx_behavior_tree_demo(void) {

    struct k_behavior_tree *tree = k_behavior_tree_create();

    struct k_behavior_tree_node *root = k_behavior_tree_get_root(tree);

    struct k_behavior_tree_node *seq = k_behavior_tree_add_sequence(root);

    {
        struct k_behavior_tree_node *seq_1 = k_behavior_tree_add_sequence(seq);
        k_behavior_tree_add_action(seq_1, &count, action_count);
        k_behavior_tree_add_action(seq_1, &count, action_log_count);
        k_behavior_tree_add_condition(seq_1, &count, condition_count);
        k_behavior_tree_add_action(seq_1, "hello", action_log);
    }

    {
        struct k_behavior_tree_node *seq_2 = k_behavior_tree_add_sequence(seq);
        k_behavior_tree_add_action(seq_2, "world", action_log);
    }
}
