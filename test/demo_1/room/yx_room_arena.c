#include <limits.h>

#include "./yx_room_arena.h"

#include "../sprite/yx_spr.h"
#include "../object/yx_obj.h"

static void set_debug(void *data) {

    if (k_key_pressed('B')) {

        if (k_key_down(K_KEY_LEFT_SHIFT)) {
            k_collision_set_debug(YX_COLLISION_GROUP_BUBBLE, 0);
            k_camera_set_debug(0);
        }
        else {
            k_collision_set_debug(YX_COLLISION_GROUP_BUBBLE, 1);
            k_camera_set_debug(1);
        }
    }
}

static int init_arena_room(void *params) {
    (void)params;

    struct yx_room_arena *room_arena = k_room_get_data();

    k_room_add_collision_manager();

    k_room_add_camera();

    k_room_add_step_callback(NULL, set_debug);
    k_room_add_draw_callback(NULL, k_clean_room_canvas, INT_MIN, INT_MIN);
    k_room_add_draw_callback(NULL, k_draw_grid, INT_MIN, INT_MIN + 1);

    {
        struct yx_obj_bubble_maker_config config;
        yx_craete_bubble_maker(&config);
    }

    {
        struct yx_obj_player_config config;
        config.x = 300;
        config.y = 400;
        config.spr_idle = yx_spr_ynx_idle;
        config.spr_run  = yx_spr_ynx_run;
        room_arena->player = yx_create_player(&config);
    }

    {
        struct yx_obj_rival_config config;
        config.x = 800;
        config.y = 600;
        config.spr_idle = yx_spr_liliko_idle;
        config.spr_run  = yx_spr_liliko_run;
        yx_create_rival(&config);
    }

    {
        struct yx_obj_rival_config config;
        config.x = 400;
        config.y = 300;
        config.spr_idle = yx_spr_liliko_idle;
        config.spr_run  = yx_spr_liliko_run;
        yx_create_rival(&config);
    }

    {
        struct yx_obj_rival_config config;
        config.x = 600;
        config.y = 500;
        config.spr_idle = yx_spr_liliko_idle;
        config.spr_run  = yx_spr_liliko_run;
        yx_create_rival(&config);
    }

    // yx_test_bt();

    return 0;
}

static void enter_arena_room(void) {
    struct yx_room_arena *room_arena = k_room_get_data();

    struct yx_obj_player *player = k_object_get_data(room_arena->player);
    k_view_set_position(player->x, player->y);
}

struct k_room *yx_create_arena_room(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w    = 1920 * 1.5f;
    config.room_h    = 1080 * 1.5f;
    config.data_size = sizeof(struct yx_room_arena);
    config.fn_init   = init_arena_room;
    config.fn_enter  = enter_arena_room;

    return k_room_create(&config, NULL);
}
