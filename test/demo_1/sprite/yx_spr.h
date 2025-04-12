#ifndef YX_SPR_H
#define YX_SPR_H

#include "k_game/core/k_sprite.h"

extern struct k_sprite *yx_spr_ynx_run;
extern struct k_sprite *yx_spr_ynx_idle;
int yx_load_spr_ynx(void);

extern struct k_sprite *yx_spr_liliko_run;
extern struct k_sprite *yx_spr_liliko_idle;
int yx_load_spr_liliko(void);

extern struct k_sprite *yx_spr_iris_gun;
extern struct k_sprite *yx_spr_iris_bullet;
int yx_load_spr_iris(void);

extern struct k_sprite *yx_spr_bubble_appear;
extern struct k_sprite *yx_spr_bubble_float;
extern struct k_sprite *yx_spr_bubble_pop;
int yx_load_spr_bubble(void);

#endif
