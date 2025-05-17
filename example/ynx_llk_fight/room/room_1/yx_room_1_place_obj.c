#include <math.h>
#include <limits.h>

#include "k_game.h"

#include "sprite/yx_spr.h"

#include "object/player/yx_obj_player.h"
#include "object/rival/yx_obj_rival.h"

#include "room/room_1/yx_room_1.h"

int yx__room_1_on_create_place_obj(void) {

    {
        struct yx_obj_player_config config;
        config.x = 60.0f;
        config.y = 60.0f;
        yx_obj_player_create(&config);
    }

    {
        struct yx_obj_rival_config config;
        config.x = 100.0f;
        config.y = 100.0f;
        yx_obj_rival_create(&config);
    }

    return 0;
}
