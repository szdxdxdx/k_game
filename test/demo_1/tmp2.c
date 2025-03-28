#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum k_behavior_tree_state fn_log(void *data) {
    printf("hello");
    return K_BT_SUCCESS;
}

static void tmp() {


    struct k_behavior_tree *tree = k_object_add_behavior_tree(object);
    struct k_bt_node *root = k_behavior_tree_root(tree);
    struct k_bt_node *action = k_bt_add_action(root, fn_log);

    k_
}

#if 1

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    tmp();

    return 0;
}

#endif

