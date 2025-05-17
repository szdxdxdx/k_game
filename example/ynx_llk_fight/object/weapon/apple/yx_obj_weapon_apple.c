
#define K_LOG_TAG "yx:object:weapon_apple"
#include "k_log.h"

#include "k_game.h"

#include "object/weapon/apple/yx_obj_weapon_apple.h"
#include "sprite/yx_spr.h"
#include "config/yx_config_z_index.h"

static void yx__obj_weapon_apple_fn_set_position(struct yx_obj_weapon_i *weapon, float x, float y, int z) {
    struct yx_obj_weapon_apple *apple = container_of(weapon, struct yx_obj_weapon_apple, weapon);
    apple->x = x;
    apple->y = y;
    k_sprite_renderer_set_z_layer(apple->spr_rdr, z);
}

static void yx__obj_weapon_apple_fn_aim_at(struct yx_obj_weapon_i *weapon, float x, float y) {
    struct yx_obj_weapon_apple *apple = container_of(weapon, struct yx_obj_weapon_apple, weapon);
    apple->aim_x = x;
    apple->aim_y = y;
}

static void yx__obj_weapon_apple_on_key_down(struct yx_obj_weapon_i *weapon) {
    (void)weapon;
}

static void yx__obj_weapon_apple_on_key_held(struct yx_obj_weapon_i *weapon) {
    (void)weapon;
}

static void yx__obj_weapon_apple_on_key_up(struct yx_obj_weapon_i *weapon) {
    (void)weapon;
}

static struct yx_obj_weapon_v_tbl yx__obj_weapon_apple_v_tbl = {
    .fn_set_position = yx__obj_weapon_apple_fn_set_position,
    .fn_aim_at = yx__obj_weapon_apple_fn_aim_at,
    .on_key_down = yx__obj_weapon_apple_on_key_down,
    .on_key_held = yx__obj_weapon_apple_on_key_held,
    .on_key_up   = yx__obj_weapon_apple_on_key_up,
};

struct yx_obj_weapon_i *yx_obj_weapon_apple_create(void) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_weapon_apple));
    if (NULL == object)
        return NULL;

    struct yx_obj_weapon_apple *apple = k_object_get_data(object);
    struct yx_obj_weapon_i *weapon = &apple->weapon;

    weapon->object = object;
    weapon->v_tbl = &yx__obj_weapon_apple_v_tbl;

    apple->x = 0;
    apple->y = 0;
    apple->aim_x = 0;
    apple->aim_y = 0;

    {
        struct k_sprite_renderer_config config;
        config.x       = &apple->x;
        config.y       = &apple->y;
        config.sprite  = yx_spr_weapon_apple;
        config.z_group = YX_CONFIG_Z_GROUP_WEAPON;
        config.z_layer = 0;
        apple->spr_rdr = k_object_add_sprite_renderer(object, &config);
        if (NULL == apple->spr_rdr)
            goto err;
    }

    return &apple->weapon;

err:
    k_log_error("failed to create object: weapon apple");
    return NULL;
}
