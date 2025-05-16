#ifndef YX_OBJ_WEAPON_I_H
#define YX_OBJ_WEAPON_I_H

struct yx_obj_weapon;

struct yx_obj_weapon_v_tbl;

struct yx_obj_weapon_v_tbl {
    void (*fn_aim_at)(struct yx_obj_weapon *self, float x, float y);
};

#endif
