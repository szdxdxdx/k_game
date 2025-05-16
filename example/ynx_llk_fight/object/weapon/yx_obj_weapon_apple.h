#ifndef YX_OBJ_WEAPON_APPLE_H
#define YX_OBJ_WEAPON_APPLE_H

#include "k_game/core/k_game_fwd.h"

struct yx_obj_weapon_apple {
    struct k_object *object;

    float x;
    float y;

    struct k_sprite_renderer *spr_rdr;
};

#endif
