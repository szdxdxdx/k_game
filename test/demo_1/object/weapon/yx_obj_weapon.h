#ifndef YX_OBJ_WEAPON_H
#define YX_OBJ_WEAPON_H

#include "k_game.h"

struct yx_obj_weapon {

    struct k_float_vec2 *position;
};

struct yx_obj_weapon_config {

    struct k_float_vec2 *position;
};

struct k_object *yx_obj_weapon_create(const struct yx_obj_weapon_config *config);

void yx_obj_weapon_destroy(struct k_object *obj_weapon);

#endif
