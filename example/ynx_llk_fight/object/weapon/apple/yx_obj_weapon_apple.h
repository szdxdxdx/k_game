#ifndef YX_OBJ_WEAPON_APPLE_H
#define YX_OBJ_WEAPON_APPLE_H

#include "k_game.h"

#include "object/weapon/yx_obj_weapon_i.h"

struct yx_obj_weapon_apple {

    struct yx_obj_weapon_i weapon;

    float x;
    float y;

    float aim_x;
    float aim_y;

    struct k_sprite_renderer *spr_rdr;
};

struct yx_obj_weapon_i *yx_obj_weapon_apple_create(void);

struct yx_obj_bullet_apple {

    float x;
    float y;

    float vx;
    float vy;

    float rotation_speed;

    struct k_sprite_renderer *spr_rdr;
};

#endif
