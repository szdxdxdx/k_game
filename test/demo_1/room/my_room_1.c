#include <limits.h>

#include "./my_room.h"
#include "../object/my_object.h"

int fn_room_init(void *params) {
    (void)params;

    k_room_add_collision_manager();

    k_room_add_draw_callback(NULL, k_room_clean_canvas, INT_MIN);
    k_room_add_draw_callback(NULL, k_room_draw_grid, INT_MIN + 1);

    my_player_create(300.0f, 300.0f);
   // my_enemy_create(500.0f, 300.0f);
   // my_bubble_create(500.0f, 300.0f);
    return 0;
}

struct k_room *my_room_1_create(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_init = fn_room_init;

    return k_room_create(&config, NULL);
}
