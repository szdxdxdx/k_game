#ifndef MY_DEMO1_SPRITE_PUBLIC_H
#define MY_DEMO1_SPRITE_PUBLIC_H

struct k_sprite;

/* region [player] */

int my_spr_player_load(void);

extern struct k_sprite *my_spr_player_run;
extern struct k_sprite *my_spr_player_idle;

int my_spr_player_bullet_load(void);

extern struct k_sprite *my_spr_player_bullet;

/* endregion */

/* region [enemy] */

int my_spr_enemy_load(void);

extern struct k_sprite *my_spr_enemy_attack;

/* endregion */

/* region [ynx] */

int my_spr_ynx_load(void);

extern struct k_sprite *my_spr_ynx_run;
extern struct k_sprite *my_spr_ynx_idle;

/* endregion */

#endif
