#ifndef YX_FX_FIGHTER_APPEAR_H
#define YX_FX_FIGHTER_APPEAR_H

#include "k_game.h"

struct yx_fx_fighter_appear {
    float x;
    float y;
    struct k_sprite_renderer *spr_rdr;
    struct k_callback *cb_step;
    float timer;
};

struct yx_fx_fighter_appear *yx_fx_fighter_appear_create(float x, float y);

#endif
