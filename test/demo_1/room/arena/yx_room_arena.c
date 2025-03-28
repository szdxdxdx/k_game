#include <limits.h>

#include "../_internal.h"

#include "../../sprite/_public.h"
#include "../../object/_public.h"

static void set_debug(void *data) {

    if (k_key_pressed('B')) {

        if (k_key_down(K_KEY_LEFT_SHIFT))
            k_collision_set_debug(YX_COLLISION_GROUP_BUBBLE, 0);
        else
            k_collision_set_debug(YX_COLLISION_GROUP_BUBBLE, 1);
    }
}

int fn_room_init(void *params) {
    (void)params;

    /* region [component_manager] */

    k_add_room_collision_manager();

    k_add_room_step_callback(NULL, set_debug);

    /* endregion */

    /* region [room_callback] */

    k_add_room_draw_callback(NULL, k_clean_room_canvas, INT_MIN, INT_MIN);
    k_add_room_draw_callback(NULL, k_draw_grid, INT_MIN, INT_MIN + 1);

    /* endregion */

    /* region [create_bubble_maker] */

    {
        struct yx_obj_bubble_maker_config config;
        //yx_obj_bubble_maker_create(&config);
    }

    /* endregion */

    /* region [create_player] */

    {
        struct yx_obj_player_config config;
        config.x = 300;
        config.y = 300;
        config.spr_idle = yx_spr_ynx_idle;
        config.spr_run  = yx_spr_ynx_run;
        yx_player_create(&config);
    }

    /* endregion */

    /* region [tmp] */

    {
        struct k_object *object = k_object_create(0);
    }

    /* endregion */

    return 0;
}

struct k_room *yx_room_arena_create(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_init = fn_room_init;

    return k_room_create(&config, NULL);
}
