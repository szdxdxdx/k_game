#include <limits.h>

#include "./_internal.h"

#include "../sprite/_public.h"
#include "../object/_public.h"

int fn_room_init(void *params) {
    (void)params;

    /* region [component_manager] */

    k_room_add_collision_manager();

    /* endregion */

    /* region [room_callback] */

    k_room_add_draw_callback(NULL, k_room_clean_canvas, INT_MIN, INT_MIN);
    k_room_add_draw_callback(NULL, k_room_draw_grid,    INT_MIN, INT_MIN + 1);

    /* endregion */

    /* region [create_bubble_maker] */

    {
        struct yx_obj_bubble_maker_config config;
        yx_obj_bubble_maker_create(&config);
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

    if (0) {
        struct yx_obj_player_config config;
        config.x = 500;
        config.y = 500;
        config.spr_idle = yx_spr_liliko_idle;
        config.spr_run  = yx_spr_liliko_run;
        yx_player_create(&config);
    }

    /* endregion */

    yx_bubble_create(500.0f, 300.0f);
    return 0;
}

struct k_room *yx_room_1_create(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_init = fn_room_init;

    return k_room_create(&config, NULL);
}
