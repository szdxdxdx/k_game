#ifndef MY_OBJ_PLAYER_H
#define MY_OBJ_PLAYER_H

#include "k_game.h"

enum my_player_state {
    player_idle,
    player_run,
};

struct my_player {
    float x;
    float y;

    float next_x;
    float next_y;

    int state;
    int face;

    struct k_component *WASD;
    struct k_sprite_renderer *spr_rdr;
};

struct k_object *my_player_create(float x, float y);

#endif
