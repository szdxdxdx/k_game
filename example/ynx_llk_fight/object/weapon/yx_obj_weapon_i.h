#ifndef YX_OBJ_WEAPON_I_H
#define YX_OBJ_WEAPON_I_H

#include "k_game/core/k_game_fwd.h"

#include "utils/yx_float_vec.h"

struct yx_obj_weapon_i;
struct yx_obj_weapon_v_tbl;

struct yx_obj_weapon_i {
    struct k_object *object;
    struct yx_obj_weapon_v_tbl *v_tbl;
};

struct yx_obj_weapon_v_tbl {

    void (*fn_set_position)(struct yx_obj_weapon_i *weapon, float x, float y, int z);

    void (*fn_aim_at)(struct yx_obj_weapon_i *weapon, float x, float y);

    void (*on_key_down)(struct yx_obj_weapon_i *weapon);
    void (*on_key_held)(struct yx_obj_weapon_i *weapon);
    void (*on_key_up)(struct yx_obj_weapon_i *weapon);
};

/* ------------------------------------------------------------------------ */

static inline void yx_obj_weapon_set_position(struct yx_obj_weapon_i *weapon, float x, float y, int z) {
    weapon->v_tbl->fn_set_position(weapon, x, y, z);
}

static inline void yx_obj_weapon_aim_at(struct yx_obj_weapon_i *weapon, float x, float y) {
    weapon->v_tbl->fn_aim_at(weapon, x, y);
}

static inline void yx_obj_weapon_on_key_down(struct yx_obj_weapon_i *weapon) {
    weapon->v_tbl->on_key_down(weapon);
}

static inline void yx_obj_weapon_on_key_held(struct yx_obj_weapon_i *weapon) {
    weapon->v_tbl->on_key_held(weapon);
}

static inline void yx_obj_weapon_on_key_up(struct yx_obj_weapon_i *weapon) {
    weapon->v_tbl->on_key_up(weapon);
}

/* ------------------------------------------------------------------------ */

struct yx_obj_bullet_i;
struct yx_obj_bullet_v_tbl;
struct yx_obj_bullet_on_hit_result;

struct yx_obj_bullet_i {
    struct k_object *object;
    struct yx_obj_bullet_v_tbl *v_tbl;
};

struct yx_obj_bullet_v_tbl {
    void (*on_hit)(struct yx_obj_bullet_i *bullet, struct yx_obj_bullet_on_hit_result *get_result);
};

/* ------------------------------------------------------------------------ */

struct yx_obj_bullet_on_hit_result {
    float vx_knockback;
    float vy_knockback;
    float damage;
};

static inline void yx_obj_bullet_on_hit(struct yx_obj_bullet_i *bullet, struct yx_obj_bullet_on_hit_result *get_result) {
    bullet->v_tbl->on_hit(bullet, get_result);
}

/* ------------------------------------------------------------------------ */

#endif
