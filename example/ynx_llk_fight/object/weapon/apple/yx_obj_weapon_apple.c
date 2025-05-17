
#define K_LOG_TAG "yx:object:weapon_apple"

#include <math.h>
#include "k_log.h"

#include "k_game.h"

#include "object/weapon/apple/yx_obj_weapon_apple.h"
#include "sprite/yx_spr.h"
#include "config/yx_config_z_index.h"
#include "utils/yx_math.h"

/* region [bullet_apple] */

static void yx__obj_bullet_apple_on_step(struct k_object *object) {
    struct yx_obj_bullet_apple *bullet_apple = k_object_get_data(object);

    float dt = k_time_get_step_delta();

    bullet_apple->x += bullet_apple->vx * dt;
    bullet_apple->y += bullet_apple->vy * dt;

    float padding = -30.0f;
    struct k_float_rect room_rect = {
        .x = padding,
        .y = padding,
        .w = k_room_get_w() - padding,
        .h = k_room_get_h() - padding
    };
    if ( ! yx_point_in_rect(bullet_apple->x, bullet_apple->y, &room_rect)) {
        k_object_destroy(object);
        return;
    }

    float angle = k_sprite_renderer_get_rotation(bullet_apple->spr_rdr);
    angle += dt * bullet_apple->rotation_speed;
    k_sprite_renderer_rotate(bullet_apple->spr_rdr, angle);
}

static struct yx_obj_bullet_apple *yx__obj_bullet_apple_create(struct yx_obj_weapon_apple *weapon_apple) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_bullet_apple));
    if (NULL == object)
        goto err;

    struct yx_obj_bullet_apple *bullet = k_object_get_data(object);
    bullet->x = weapon_apple->x;
    bullet->y = weapon_apple->y;

    bullet->rotation_speed = yx_rand(300.0f, 800.0f);

    float sin_angle;
    float cos_angle;
    yx_calc_vector_direction(weapon_apple->x, weapon_apple->y, weapon_apple->aim_x, weapon_apple->aim_y, &cos_angle, &sin_angle);

    float speed = yx_rand(350.0f, 450.0f);
    bullet->vx = cos_angle * speed;
    bullet->vy = sin_angle * speed;

    {
        struct k_sprite_renderer_config config;
        config.x       = &bullet->x;
        config.y       = &bullet->y;
        config.sprite  = yx_spr_weapon_apple;
        config.z_group = YX_CONFIG_Z_GROUP_BULLET;
        config.z_layer = 0;
        bullet->spr_rdr = k_object_add_sprite_renderer(object, &config);
        if (NULL == bullet->spr_rdr)
            goto err;

        float angle = k_sprite_renderer_get_rotation(weapon_apple->spr_rdr);
        k_sprite_renderer_rotate(bullet->spr_rdr, angle);
    }

    if (NULL == k_object_add_step_callback(object, yx__obj_bullet_apple_on_step))
        goto err;

    return bullet;

err:
    k_object_destroy(object);
    k_log_error("failed to create object: bullet apple");
    return NULL;
}

/* endregion */

/* region [weapon_apple] */

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

    if (apple->aim_x != apple->x) {
        float angle = atanf((apple->aim_y - apple->y) / (apple->aim_x - apple->x));
        angle *= 180.0f / 3.1415926f;

        k_sprite_renderer_rotate(apple->spr_rdr, angle);

        if (apple->aim_x < apple->x) {
            k_sprite_renderer_flip_x(apple->spr_rdr, 1);
        }
        else {
            k_sprite_renderer_flip_x(apple->spr_rdr, 0);
        }
    }
}

static void yx__obj_weapon_apple_attack(struct yx_obj_weapon_apple *weapon_apple) {

    if (weapon_apple->ammo > 0) {
        yx__obj_bullet_apple_create(weapon_apple);
        weapon_apple->attack_cd_timer = weapon_apple->attack_cd_time;
        weapon_apple->ammo -= 1;

        if (0 == weapon_apple->ammo) {
            k_sprite_renderer_scale_x(weapon_apple->spr_rdr, 0.0f);
            k_sprite_renderer_scale_y(weapon_apple->spr_rdr, 0.0f);
        }
    }
}

static void yx__obj_weapon_apple_on_key_down(struct yx_obj_weapon_i *weapon) {
    struct yx_obj_weapon_apple *weapon_apple = container_of(weapon, struct yx_obj_weapon_apple, weapon);

    yx__obj_weapon_apple_attack(weapon_apple);
}

static void yx__obj_weapon_apple_on_key_held(struct yx_obj_weapon_i *weapon) {
    struct yx_obj_weapon_apple *weapon_apple = container_of(weapon, struct yx_obj_weapon_apple, weapon);

    if (0.0f == weapon_apple->attack_cd_timer) {
        yx__obj_weapon_apple_attack(weapon_apple);
    }
}

static void yx__obj_weapon_apple_on_key_up(struct yx_obj_weapon_i *weapon) {
    (void)weapon;
}

static struct yx_obj_weapon_v_tbl yx__obj_weapon_apple_v_tbl = {
    .fn_set_position = yx__obj_weapon_apple_fn_set_position,
    .fn_aim_at   = yx__obj_weapon_apple_fn_aim_at,
    .on_key_down = yx__obj_weapon_apple_on_key_down,
    .on_key_held = yx__obj_weapon_apple_on_key_held,
    .on_key_up   = yx__obj_weapon_apple_on_key_up,
};

static void yx__obj_weapon_apple_on_step(struct k_object *object) {
    struct yx_obj_weapon_apple *weapon_apple = k_object_get_data(object);

    float dt = k_time_get_step_delta();

    if (weapon_apple->ammo > 0 && weapon_apple->attack_cd_timer > 0.0f) {
        weapon_apple->attack_cd_timer -= dt;

        if (weapon_apple->attack_cd_timer <= 0) {
            weapon_apple->attack_cd_timer = 0.0f;
            k_sprite_renderer_scale_x(weapon_apple->spr_rdr, 1.0f);
            k_sprite_renderer_scale_y(weapon_apple->spr_rdr, 1.0f);
        }
        else {
            float scale = 1.0f - weapon_apple->attack_cd_timer / weapon_apple->attack_cd_time;
            k_sprite_renderer_scale_x(weapon_apple->spr_rdr, scale);
            k_sprite_renderer_scale_y(weapon_apple->spr_rdr, scale);
        }
    }

    weapon_apple->ammo_timer += dt;
    if (weapon_apple->ammo_timer >= weapon_apple->ammo_cd_time) {
        weapon_apple->ammo += 1;
        weapon_apple->ammo_timer -= weapon_apple->ammo_cd_time;
    }
}

static void yx__obj_weapon_apple_on_draw(struct k_object *object) {
    struct yx_obj_weapon_apple *weapon_apple = k_object_get_data(object);

    k_canvas_set_draw_color(0xffffffff);
    k_canvas_ui_printf(NULL, 0, 0, "%zu", weapon_apple->ammo);
}

struct yx_obj_weapon_i *yx_obj_weapon_apple_create(void) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_weapon_apple));
    if (NULL == object)
        return NULL;

    struct yx_obj_weapon_apple *weapon_apple = k_object_get_data(object);
    struct yx_obj_weapon_i *weapon = &weapon_apple->weapon;

    weapon->object = object;
    weapon->v_tbl = &yx__obj_weapon_apple_v_tbl;

    weapon_apple->x = 0;
    weapon_apple->y = 0;
    weapon_apple->aim_x = 0;
    weapon_apple->aim_y = 0;

    weapon_apple->attack_cd_time = 0.25f;
    weapon_apple->attack_cd_timer = 0;

    weapon_apple->ammo = 10;
    weapon_apple->ammo_timer = 0.0f;
    weapon_apple->ammo_cd_time = 0.8f;

    {
        struct k_sprite_renderer_config config;
        config.x       = &weapon_apple->x;
        config.y       = &weapon_apple->y;
        config.sprite  = yx_spr_weapon_apple;
        config.z_group = YX_CONFIG_Z_GROUP_WEAPON;
        config.z_layer = 0;
        weapon_apple->spr_rdr = k_object_add_sprite_renderer(object, &config);
        if (NULL == weapon_apple->spr_rdr)
            goto err;
    }

    if (NULL == k_object_add_step_callback(object, yx__obj_weapon_apple_on_step))
        goto err;

    k_object_add_draw_callback(object, yx__obj_weapon_apple_on_draw, 0, 0);

    return &weapon_apple->weapon;

err:
    k_object_destroy(object);
    k_log_error("failed to create object: weapon apple");
    return NULL;
}

/* endregion */
