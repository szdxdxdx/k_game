#include <stdio.h>
#include <stdlib.h>

#include "k_str_map.h"

#include "k_game.h"

#include "./yx_room_arena.h"

static void step_div(void *data) {
    printf("~\n");
}

static struct k_str_map *blackboard;

enum k_behavior_tree_status is_mouse_moved(void *data) {

    int *x = k_str_map_get(blackboard, "x");
    int *y = k_str_map_get(blackboard, "y");

    if (*x != k_mouse_x() || *y != k_mouse_y()) {
        *x = k_mouse_x();
        *y = k_mouse_y();
        return K_BT_SUCCESS;
    } else {
        return K_BT_FAILURE;
    }
}

enum k_behavior_tree_status log_mouse_pos(void *data) {

    int *x = k_str_map_get(blackboard, "x");
    int *y = k_str_map_get(blackboard, "y");

    printf("mouse pos: %d, %d\n", *x, *y);

    return K_BT_SUCCESS;
}

void yx_test_bt(void) {

    //k_add_room_step_end_callback(NULL, step_div);

    struct k_str_map_config config = {
        .fn_malloc = malloc,
        .fn_free   = free,
    };
    blackboard = k_str_map_create(&config);

    int *x = k_str_map_put(blackboard, "x", sizeof(int));
    *x = k_mouse_x();
    int *y = k_str_map_put(blackboard, "y", sizeof(int));
    *y = k_mouse_y();

    struct k_behavior_tree_builder *b;
    struct k_behavior_tree *tree;

    k_bt_builder(&tree, b)
    {
        k_bt_sequence(b)
        {
            k_bt_condition(b, NULL, is_mouse_moved);
            k_bt_action(b, NULL, log_mouse_pos, NULL);
        }
    }
}
