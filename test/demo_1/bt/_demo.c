#include <stdio.h>

#include "./_internal.h"

static enum yx_behavior_tree_status action_log(void *data) {

    printf("%s\n", (const char *)data);

    return YX_BT_SUCCESS;
}

void yx_behavior_tree_demo(void) {

    struct yx_behavior_tree *tree = yx_behavior_tree_create();

    struct yx_behavior_tree_node *root = yx_behavior_tree_get_root(tree);

    struct yx_behavior_tree_node *seq = yx_behavior_tree_add_sequence(root);
    yx_behavior_tree_add_action(seq, "1", action_log);
    yx_behavior_tree_add_action(seq, "2", action_log);
    yx_behavior_tree_add_action(seq, "3", action_log);
}
