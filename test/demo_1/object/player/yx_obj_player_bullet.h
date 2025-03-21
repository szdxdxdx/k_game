#ifndef YX__OBJ_PLAYER_BULLET_H
#define YX__OBJ_PLAYER_BULLET_H

#include "k_game.h"

struct yx_obj_player_bullet {
    float speed;
    float x;
    float y;
};

struct k_object *yx_player_bullet_create(float x, float y, int face);

#endif
