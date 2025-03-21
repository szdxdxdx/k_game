#ifndef YX__OBJ_PLAYER_H
#define YX__OBJ_PLAYER_H

#include "k_game.h"

enum yx_obj_player_state {
    player_idle,
    player_run,
};

struct yx_obj_player {
    float x;
    float y;

    float next_x;
    float next_y;

    int state;
    int face;

    struct k_component *WASD;
    struct k_sprite_renderer *spr_rdr;
};

struct k_object *yx_player_create(float x, float y);

#endif
