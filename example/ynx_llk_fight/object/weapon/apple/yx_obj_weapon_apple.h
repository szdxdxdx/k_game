#ifndef YX_OBJ_WEAPON_APPLE_H
#define YX_OBJ_WEAPON_APPLE_H

#include "k_game.h"

#include "object/weapon/yx_obj_weapon_i.h"

struct yx_obj_weapon_apple {
    struct yx_obj_weapon_i super;

    float x;
    float y;

    float aim_x;
    float aim_y;

    float attack_cd_time;
    float attack_cd_timer;

    size_t ammo;
    float ammo_cd_time;
    float ammo_timer;

    struct k_sprite_renderer *spr_rdr;
};

struct yx_obj_weapon_i *yx_obj_weapon_apple_create(void);

struct yx_obj_bullet_apple {
    struct yx_obj_bullet_i super;

    float x;
    float y;

    float vx;
    float vy;

    float rotation_speed;

    struct k_sprite_renderer *spr_rdr;
    struct k_collision_box *hit_box;
};

#endif
