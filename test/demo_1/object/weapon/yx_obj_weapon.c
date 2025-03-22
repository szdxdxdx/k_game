#include "../_internal.h"

struct k_object *yx_obj_weapon_create(const struct yx_obj_weapon_config *config) {
    struct k_object *object = k_object_create(sizeof(struct yx_obj_weapon));

    struct yx_obj_weapon *weapon = k_object_get_data(object);
    weapon->position = config->position;

    return object;
}

void yx_obj_weapon_destroy(struct k_object *obj_weapon) {

    if (NULL != obj_weapon)
        k_object_destroy(obj_weapon);
}
