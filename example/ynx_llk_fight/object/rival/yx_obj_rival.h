#ifndef YX_OBJ_RIVAL_H
#define YX_OBJ_RIVAL_H

#include "k_game/core/k_game_fwd.h"
#include "utils/yx_float_vec.h"

struct yx_obj_rival_config {
    float x;
    float y;
};

struct yx_obj_rival *yx_obj_rival_create(const struct yx_obj_rival_config *config);

/* ------------------------------------------------------------------------ */

struct yx_obj_rival {
    struct k_object *object;

    float x;
    float y;

    struct yx_float_vec2 v_move;

    struct k_sprite_renderer *spr_rdr;
    struct k_sprite *spr_idle;
    struct k_sprite *spr_run;

    struct yx_obj_weapon_i *weapon;
};

int yx__obj_rival_on_create_add_movement(struct yx_obj_rival *rival);

int yx__obj_rival_on_create_add_collision(struct yx_obj_rival *rival);

int yx__obj_rival_on_create_add_draw(struct yx_obj_rival *rival);

#endif
