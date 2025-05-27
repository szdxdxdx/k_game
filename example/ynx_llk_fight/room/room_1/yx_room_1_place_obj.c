#include <math.h>
#include <limits.h>

#include "k_game.h"

#include "config/yx_config_collision_group.h"

#include "sprite/yx_spr.h"

#include "object/fighter/player/yx_obj_player.h"
#include "object/fighter/rival/yx_obj_rival.h"

#include "room/room_1/yx_room_1.h"
#include "utils/yx_math.h"
#include "object/particle/yx_fx_fighter_appear.h"

int yx__room_1_on_create_place_obj(void) {

    struct yx_room_1 *room_1 = k_room_get_data();

    {
        struct yx_obj_player_config config;
        config.x = 100.0f;
        config.y = 100.0f;
        yx_obj_player_create(&config);
    }

    {
        float x_min = 0.0f;
        float y_min = 0.0f;
        float x_max = k_room_get_w();
        float y_max = k_room_get_h();

        int i = 0;
        for (; i < 10; ++i) {
            float x = yx_rand(x_min, x_max);
            float y = yx_rand(y_min, y_max);
            yx_obj_rival_spawn(x, y);
        }
    }

    return 0;
}
