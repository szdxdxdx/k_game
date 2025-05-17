#ifndef YX_OBJ_WEAPON_I_H
#define YX_OBJ_WEAPON_I_H

#include "k_game/core/k_game_fwd.h"

struct yx_obj_weapon_i;
struct yx_obj_weapon_v_tbl;

struct yx_obj_weapon_i {

    struct k_object *object;

    float x;
    float y;

    struct yx_obj_weapon_v_tbl *v_tbl;
};

struct yx_obj_weapon_v_tbl {

    void (*fn_set_position)(struct yx_obj_weapon_i *weapon, float x, float y);
    void (*fn_set_z_layer)(struct yx_obj_weapon_i *weapon, int z_layer);

    void (*fn_aim_at)(struct yx_obj_weapon_i *weapon, float x, float y);

    void (*on_key_down)(struct yx_obj_weapon_i *weapon);
    void (*on_key_held)(struct yx_obj_weapon_i *weapon);
    void (*on_key_release)(struct yx_obj_weapon_i *weapon);
};

#endif
