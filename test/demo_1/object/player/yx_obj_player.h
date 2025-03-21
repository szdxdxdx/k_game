#ifndef YX__OBJ_PLAYER_H
#define YX__OBJ_PLAYER_H

#include "k_game.h"

struct yx_obj_player_config {
    float x;
    float y;

    struct k_sprite *spr_idle;
    struct k_sprite *spr_run;
};

struct k_object *yx_player_create(const struct yx_obj_player_config *config);

enum yx_obj_player_state {
    YX_OBJ_PLAYER_STATE_IDLE,
    YX_OBJ_PLAYER_STATE_RUN,
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

    struct k_sprite *spr_idle;
    struct k_sprite *spr_run;
};

#endif
