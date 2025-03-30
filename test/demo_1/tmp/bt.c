#include <stdio.h>

#include "k_game.h"

static enum k_behavior_tree_status action_count(void *data) {
    int *count = data;

    *count += 1;
    printf("count++\n");
    return K_BT_SUCCESS;
}

static enum k_behavior_tree_status condition_count_lt_1(void *data) {
    int *count = data;

    if (*count < 1) {
        printf("count < 1 is true\n");
        return K_BT_SUCCESS;
    }
    else {
        printf("count < 1 is false\n");
        return K_BT_FAILURE;
    }
}

static enum k_behavior_tree_status condition_count_gt_3(void *data) {
    int *count = data;

    if (*count > 3) {
        printf("count > 3 is true\n");
        return K_BT_SUCCESS;
    }
    else {
        printf("count > 3 is false\n");
        return K_BT_FAILURE;
    }
}

static enum k_behavior_tree_status action_log_count(void *data) {
    int *count = data;

    if (*count >= 8)
        *count = 0;
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

    /*
    struct k_behavior_tree_node *root = k_behavior_tree_get_root(tree);

    struct k_behavior_tree_node *seq_1 = k_behavior_tree_add_sequence(root);
    {
        k_behavior_tree_add_action(seq_1, &count, action_count);
        k_behavior_tree_add_action(seq_1, &count, action_log_count);
        struct k_behavior_tree_node *selector = k_behavior_tree_add_selector(seq_1);
        {
            k_behavior_tree_add_condition(selector, &count, condition_count_gt_3);
            k_behavior_tree_add_condition(selector, &count, condition_count_lt_1);
        }
        k_behavior_tree_add_action(seq_1, "       hello", action_log);
    }
     */

    struct k_behavior_tree_builder *b;
    k_bt_builder(tree, b)
    {
        k_bt_sequence(b)
        {
            k_bt_action(b, &count, action_count);
            k_bt_action(b, &count, action_log_count);

            k_bt_selector(b)
            {
                k_bt_condition(b, &count, condition_count_gt_3);
                k_bt_condition(b, &count, condition_count_lt_1);
            }

            k_bt_action(b, "       hello", action_log);
        }
    }
}
