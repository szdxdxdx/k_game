#include <math.h>
#include <limits.h>

#include "k_game.h"

#include "sprite/yx_spr.h"

#include "object/player/yx_obj_player.h"

#include "room/room_1/yx_room_1.h"

int yx__room_1_on_create_place_obj(void) {

    struct yx_obj_player_config player_config = {
        .spr_idle = yx_spr_ynx_idle,
        .spr_run  = yx_spr_ynx_run,
        .x = 60.0f,
        .y = 60.0f,
    };
    yx_obj_player_create(&player_config);

    return 0;
}
