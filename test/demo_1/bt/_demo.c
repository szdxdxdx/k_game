#include <stdio.h>

#include "./_internal.h"

static enum yx_behavior_tree_status action_count(void *data) {

    int *count = data;
    *count += 1;
    printf("count++\n");
    return YX_BT_SUCCESS;
}

static enum yx_behavior_tree_status condition_count(void *data) {

    int *count = data;
    if (*count > 3) {
        *count = 0;
        printf("condition = true, set count = 0\n");
        return YX_BT_SUCCESS;
    }
    else {
        printf("condition = false\n");
        return YX_BT_FAILURE;
    }
}

static enum yx_behavior_tree_status action_log_count(void *data) {

    printf("count = %d\n", *(int *)data);

    return YX_BT_SUCCESS;
}

static enum yx_behavior_tree_status action_log(void *data) {

    printf("%s\n", (const char *)data);

    return YX_BT_SUCCESS;
}

void yx_behavior_tree_demo(void) {

    struct yx_behavior_tree *tree = yx_behavior_tree_create();

    struct yx_behavior_tree_node *root = yx_behavior_tree_get_root(tree);

    struct yx_behavior_tree_node *seq = yx_behavior_tree_add_sequence(root);
    static int count = 0;
    yx_behavior_tree_add_action(seq, &count, action_count);
    yx_behavior_tree_add_action(seq, &count, action_log_count);
    yx_behavior_tree_add_condition(seq, &count, condition_count);
    yx_behavior_tree_add_action(seq, "hello", action_log);
}
