#include <math.h>
#include <limits.h>

#include "k_game.h"

#include "config/yx_config_collision_group.h"

#include "sprite/yx_spr.h"

#include "object/player/yx_obj_player.h"
#include "object/rival/yx_obj_rival.h"

#include "room/room_1/yx_room_1.h"

int yx__room_1_on_create_place_obj(void) {

    struct yx_room_1 *room_1 = k_room_get_data();

    if (0 != k_room_add_collision_manager())
        return -1;

    k_collision_set_debug(YX_CONFIG_COLLISION_GROUP_FIGHTER, 1);
    k_collision_set_debug(YX_CONFIG_COLLISION_GROUP_BULLET, 1);

    {
        struct yx_obj_player_config config;
        config.x = 100.0f;
        config.y = 100.0f;
        room_1->player = yx_obj_player_create(&config);
        if (NULL == room_1->player)
            return -1;
    }

    {
        struct yx_obj_rival_config config;
        config.x = 200.0f;
        config.y = 100.0f;
        yx_obj_rival_create(&config);
    }

    return 0;
}
