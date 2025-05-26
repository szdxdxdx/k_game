#ifndef YX_FX_FIGHTER_APPEAR_H
#define YX_FX_FIGHTER_APPEAR_H

#include "k_game.h"

struct yx_fx_fighter_appear {
    struct k_object *object;
    float x;
    float y;
    struct k_sprite_renderer *spr_rdr;
    struct k_callback *cb_step;
    float timer;

    struct k_object *data;
    void (*fn_callback)(struct k_object *data);
};

struct yx_fx_fighter_appear *yx_fx_fighter_appear_create(float x, float y, void (*fn_callback)(struct k_object *data), struct k_object *data);

#endif
