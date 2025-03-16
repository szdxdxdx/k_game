#ifndef MY_SPRITE_H
#define MY_SPRITE_H

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

#endif
