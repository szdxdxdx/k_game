#ifndef YX_OBJ_BUBBLE_H
#define YX_OBJ_BUBBLE_H

#include "k_game.h"

struct yx_obj_bubble {
    struct k_sprite_renderer *spr_rdr;

    float x;
    float y;
};

struct k_object *yx_bubble_create(float x, float y);

#endif
