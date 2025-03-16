#ifndef MY_SPRITE_H
#define MY_SPRITE_H

#include "./my_spr_player.h"
#include "./my_spr_enemy.h"

struct k_sprite;

extern struct k_sprite *my_spr_player_run;
extern struct k_sprite *my_spr_player_idle;

extern struct k_sprite *my_spr_enemy_attack;

#endif
