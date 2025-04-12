#ifndef YX_OBJ_BUBBLE_H
#define YX_OBJ_BUBBLE_H

#include "k_game.h"

struct yx_obj_bubble {
    struct k_sprite_renderer *spr_rdr;
    struct k_collision_box *collision_box;

    float x;
    float y;
};

struct k_object *yx_create_bubble(float x, float y);

void yx_pop_bubble(struct k_object *obj_bubble);

#endif
