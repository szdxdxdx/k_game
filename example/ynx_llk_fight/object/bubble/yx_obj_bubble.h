#ifndef YX_OBJ_BUBBLE_H
#define YX_OBJ_BUBBLE_H

#include "k_game.h"

enum yx_collision_group_id {
    YX_COLLISION_GROUP_BUBBLE = 456,
};

struct yx_obj_bubble {
    struct k_sprite_renderer *spr_rdr;
    struct k_collision_box *collision_box;

    float x;
    float y;
};

struct k_object *yx_obj_bubble_create(float x, float y);

void yx_obj_bubble_pop(struct k_object *obj_bubble);

#endif
