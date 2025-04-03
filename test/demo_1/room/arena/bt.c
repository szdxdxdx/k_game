#include <stdio.h>
#include <limits.h>

#include "k_game.h"

#include "./yx_room_arena.h"

static void step_div(void *data) {
    printf("~\n");
}

static enum k_behavior_tree_status log_s_s(void *data) {
    const char *s = data;

    printf("%s\n", s);
    return K_BT_SUCCESS;
}

static enum k_behavior_tree_status log_s_f(void *data) {
    const char *s = data;

    printf("%s\n", s);
    return K_BT_FAILURE;
}

static enum k_behavior_tree_status log_s_r(void *data) {
    const char *s = data;

    printf("%s\n", s);
    return K_BT_RUNNING;
}

static void interrupt(void *data) {
    const char *s = data;

    printf("interrupt %s\n", s);
}

void yx_test_bt(void) {

    k_add_room_step_end_callback(NULL, step_div);

    struct k_behavior_tree_builder *b;
    struct k_behavior_tree *tree;

    k_bt_builder(&tree, b)
    {
        k_bt_sequence(b)
        {
            k_bt_parallel(b, 1, INT_MAX)
            {
                k_bt_action(b, "a", log_s_f, interrupt);
                k_bt_action(b, "b", log_s_s, interrupt);
                k_bt_action(b, "c", log_s_f, interrupt);
                k_bt_action(b, "d", log_s_s, interrupt);
            }
            k_bt_action(b, "e", log_s_s, NULL);
        }
    }

}
