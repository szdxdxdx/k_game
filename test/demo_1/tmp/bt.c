#include <stdio.h>

#include "k_game.h"

static enum k_behavior_tree_status inc_count(void *data) {
    int *count = data;
    printf("[%20s]  ", __func__);

    *count += 1;
    printf("count++\n");
    return K_BT_SUCCESS;
}

static enum k_behavior_tree_status log_count(void *data) {
    int *count = data;
    printf("[%20s]  ", __func__);

    if (*count >= 8)
        *count = 0;
    printf("count = %d\n", *(int *)data);

    return K_BT_SUCCESS;
}

static enum k_behavior_tree_status is_count_lt_1(void *data) {
    int *count = data;
    printf("[%20s]  ", __func__);

    if (*count < 1) {
        printf("count < 1 is true\n");
        return K_BT_SUCCESS;
    }
    else {
        printf("count < 1 is false\n");
        return K_BT_FAILURE;
    }
}

static enum k_behavior_tree_status is_count_lt_3(void *data) {
    int *count = data;
    printf("[%20s]  ", __func__);

    if (*count < 3) {
        printf("count < 3 is true\n");
        return K_BT_SUCCESS;
    }
    else {
        printf("count < 3 is false\n");
        return K_BT_FAILURE;
    }
}

static enum k_behavior_tree_status log_string(void *data) {
    printf("[%20s]  ", __func__);

    printf("%s\n", (const char *)data);

    return K_BT_SUCCESS;
}

static int count = 0;

void yx_behavior_tree_demo(void) {

    struct k_behavior_tree_builder *b;
    struct k_behavior_tree *tree;

    k_bt_builder(tree, b)
    {
        k_bt_parallel(b)
        {
            k_bt_action(b, &count, inc_count);
            k_bt_action(b, &count, log_count);
            k_bt_force_success(b)
            {
                k_bt_sequence(b)
                {
                    k_bt_selector(b)
                    {
                        k_bt_condition(b, &count, is_count_lt_1);
                        k_bt_inverter(b) {
                            k_bt_condition(b, &count, is_count_lt_3);
                        }
                    }
                    k_bt_action(b, "hello", log_string);
                }
            }

            k_bt_action(b, "world", log_string);
        }
    }

    (void)tree;
}
