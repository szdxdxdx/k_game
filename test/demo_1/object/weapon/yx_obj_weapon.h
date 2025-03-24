#ifndef YX_OBJ_WEAPON_H
#define YX_OBJ_WEAPON_H

#include "k_game.h"

struct yx_obj_weapon {

    struct k_object *object;

    struct k_position *position;
    float x;
    float y;

    struct k_sprite_renderer *spr_rdr;
};

struct yx_obj_weapon_config {

    struct k_position *parent;
};

struct yx_obj_weapon *yx_obj_weapon_create(const struct yx_obj_weapon_config *config);

void yx_obj_weapon_destroy(struct yx_obj_weapon *weapon);

void yx_obj_weapon_set_z_index(struct yx_obj_weapon *weapon, int z_index);

#endif
