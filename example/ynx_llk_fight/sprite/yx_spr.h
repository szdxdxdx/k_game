#ifndef YX_SPR_H
#define YX_SPR_H

#include "k_game/core/k_sprite.h"

extern struct k_sprite *yx_spr_ynx_run;
extern struct k_sprite *yx_spr_ynx_idle;

extern struct k_sprite *yx_spr_liliko_run;
extern struct k_sprite *yx_spr_liliko_idle;

extern struct k_sprite *yx_spr_iris_gun;
extern struct k_sprite *yx_spr_iris_bullet;
extern struct k_sprite *yx_spr_iris_apple;

extern struct k_sprite *yx_spr_weapon_apple;
extern struct k_sprite *yx_spr_weapon_apple_crack;

extern struct k_sprite *yx_spr_bubble_appear;
extern struct k_sprite *yx_spr_bubble_float;
extern struct k_sprite *yx_spr_bubble_pop;

int yx_spr_load_on_game_start(void);

#endif
