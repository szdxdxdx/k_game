#ifndef YX_OBJ_WEAPON_APPLE_H
#define YX_OBJ_WEAPON_APPLE_H

#include "k_game.h"

#include "object/weapon/yx_obj_weapon_i.h"

/* region [player_weapon] */

struct yx_obj_player_weapon_apple {
    struct yx_obj_player_weapon super;

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

struct yx_obj_player_weapon *yx_obj_player_weapon_apple_create(void);

struct yx_obj_player_bullet_apple {
    struct yx_obj_player_bullet super;
    float x;
    float y;
    float vx;
    float vy;
    float rotation_speed;
    struct k_sprite_renderer *spr_rdr;
    struct k_collision_box *hit_box;
};

/* endregion */

/* region [rival_weapon] */

struct yx_obj_rival_weapon_apple {
    struct yx_obj_rival_weapon super;

    float x;
    float y;

    float aim_x;
    float aim_y;

    float attack_cd_time;
    float attack_cd_timer;

    struct k_sprite_renderer *spr_rdr;
};

struct yx_obj_rival_weapon *yx_obj_rival_weapon_apple_create(void);

struct yx_obj_rival_bullet_apple {
    struct yx_obj_rival_bullet super;
    float x;
    float y;
    float vx;
    float vy;
    float rotation_speed;
    struct k_sprite_renderer *spr_rdr;
    struct k_collision_box *hit_box;
};

/* endregion */

#endif
