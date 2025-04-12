#ifndef YX_OBJ_PLAYER_H
#define YX_OBJ_PLAYER_H

#include "k_game.h"

struct yx_obj_weapon;

struct yx_obj_player_config {
    float x;
    float y;

    struct k_sprite *spr_idle;
    struct k_sprite *spr_run;
};

struct k_object *yx_create_player(const struct yx_obj_player_config *config);

struct yx_obj_player {

    struct k_position *position;
    float x;
    float y;

    struct k_component *WASD;
    float next_x;
    float next_y;

    struct k_state_machine *state_machine;

    struct k_sprite_renderer *spr_rdr;
    struct k_sprite *spr_idle;
    struct k_sprite *spr_run;

    struct yx_obj_weapon *weapon;
};

#endif
