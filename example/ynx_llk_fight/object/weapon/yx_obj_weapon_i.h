#ifndef YX_OBJ_WEAPON_I_H
#define YX_OBJ_WEAPON_I_H

#include "k_game/core/k_game_fwd.h"

#include "utils/yx_float_vec.h"

/* region [player_weapon] */

struct yx_obj_player_weapon;
struct yx_obj_player_weapon_v_tbl;

struct yx_obj_player_weapon {
    struct k_object *object;
    struct yx_obj_player_weapon_v_tbl *v_tbl;
};

struct yx_obj_player_weapon_v_tbl {

    void (*fn_set_position)(struct yx_obj_player_weapon *weapon, float x, float y, int z);

    void (*fn_aim_at)(struct yx_obj_player_weapon *weapon, float x, float y);

    void (*on_key_down)(struct yx_obj_player_weapon *weapon);
    void (*on_key_held)(struct yx_obj_player_weapon *weapon);
    void (*on_key_up)(struct yx_obj_player_weapon *weapon);
};

/* ------------------------------------------------------------------------ */

static inline void yx_obj_player_weapon_set_position(struct yx_obj_player_weapon *weapon, float x, float y, int z) {
    weapon->v_tbl->fn_set_position(weapon, x, y, z);
}

static inline void yx_obj_player_weapon_aim_at(struct yx_obj_player_weapon *weapon, float x, float y) {
    weapon->v_tbl->fn_aim_at(weapon, x, y);
}

static inline void yx_obj_player_weapon_on_key_down(struct yx_obj_player_weapon *weapon) {
    weapon->v_tbl->on_key_down(weapon);
}

static inline void yx_obj_player_weapon_on_key_held(struct yx_obj_player_weapon *weapon) {
    weapon->v_tbl->on_key_held(weapon);
}

static inline void yx_obj_player_weapon_on_key_up(struct yx_obj_player_weapon *weapon) {
    weapon->v_tbl->on_key_up(weapon);
}

/* ------------------------------------------------------------------------ */

struct yx_obj_player_bullet;
struct yx_obj_player_bullet_v_tbl;
struct yx_obj_player_bullet_on_hit_result;

struct yx_obj_player_bullet {
    struct k_object *object;
    struct yx_obj_player_bullet_v_tbl *v_tbl;
};

struct yx_obj_player_bullet_v_tbl {
    void (*on_hit)(struct yx_obj_player_bullet *bullet, struct yx_obj_player_bullet_on_hit_result *get_result);
};

/* ------------------------------------------------------------------------ */

struct yx_obj_player_bullet_on_hit_result {
    float vx_knockback;
    float vy_knockback;
    float damage;
};

static inline void yx_obj_player_bullet_on_hit(struct yx_obj_player_bullet *bullet, struct yx_obj_player_bullet_on_hit_result *get_result) {
    bullet->v_tbl->on_hit(bullet, get_result);
}

/* ------------------------------------------------------------------------ */

/* endregion */

/* region [rival_weapon] */

struct yx_obj_rival_weapon;
struct yx_obj_rival_weapon_v_tbl;

struct yx_obj_rival_weapon {
    struct k_object *object;
    struct yx_obj_rival_weapon_v_tbl *v_tbl;
};

struct yx_obj_rival_weapon_v_tbl {
    void (*fn_set_position)(struct yx_obj_rival_weapon *weapon, float x, float y, int z);
    void (*fn_aim_at)(struct yx_obj_rival_weapon *weapon, float x, float y);
    void (*fn_attack)(struct yx_obj_rival_weapon *weapon);
};

static inline void yx_obj_rival_weapon_set_position(struct yx_obj_rival_weapon *weapon, float x, float y, int z) {
    weapon->v_tbl->fn_set_position(weapon, x, y, z);
}

static inline void yx_obj_rival_weapon_aim_at(struct yx_obj_rival_weapon *weapon, float x, float y) {
    weapon->v_tbl->fn_aim_at(weapon, x, y);
}

static inline void yx_obj_rival_weapon_attack(struct yx_obj_rival_weapon *weapon) {
    weapon->v_tbl->fn_attack(weapon);
}

/* endregion */

#endif
