#include <math.h>
#include "../_internal.h"

/* region [bullet] */

struct yx_obj_bullet {

    float velocity_x;
    float velocity_y;
    struct k_float_vec2 position;
};

static void bullet_touch_bubble(struct k_object *object) {
    struct yx_obj_bullet *bullet = k_object_get_data(object);

    float padding = 2;
    float x1 = bullet->position.x - padding;
    float y1 = bullet->position.y - padding;
    float x2 = bullet->position.x + padding;
    float y2 = bullet->position.y + padding;
    struct k_collision_box *box = k_collision_check_rectangle(YX_COLLISION_GROUP_BUBBLE, x1, y1, x2, y2);
    if (NULL != box)
        yx_bubble_pop(k_collision_box_get_object(box));
}

static void bullet_move(struct k_object *object) {
    struct yx_obj_bullet *bullet = k_object_get_data(object);

    float delta = k_get_step_delta();
    bullet->position.x += bullet->velocity_x * delta;
    bullet->position.y += bullet->velocity_y * delta;

    if (   bullet->position.x < 0 || bullet->position.x > k_get_room_width()
        || bullet->position.y < 0 || bullet->position.y > k_get_room_height()
    ) {
        k_object_destroy(object);
    }
}

static void bullet_create(struct yx_obj_weapon *weapon) {
    struct k_object *obj_bullet = k_object_create(sizeof(struct yx_obj_bullet));
    struct yx_obj_bullet *bullet = k_object_get_data(obj_bullet);

    float mouse_x = (float)k_mouse_x();
    float mouse_y = (float)k_mouse_y();
    float weapon_x = weapon->position->x;
    float weapon_y = weapon->position->y;
    float angle = atanf((mouse_y - weapon_y) / (mouse_x - weapon_x));

    float cos_angle;
    float sin_angle;
    float speed = 300.0f;
    if (mouse_x < weapon_x) {
        cos_angle = -cosf(angle);
        sin_angle = -sinf(angle);
    } else {
        cos_angle = cosf(angle);
        sin_angle = sinf(angle);

    }

    bullet->velocity_x = cos_angle * speed;
    bullet->velocity_y = sin_angle * speed;

    bullet->position.x = weapon->position->x + cos_angle * 20;
    bullet->position.y = weapon->position->y + sin_angle * 20;

    k_object_add_step_callback(obj_bullet, bullet_move);

    struct k_sprite_renderer_config renderer_config;
    renderer_config.x       = &bullet->position.x;
    renderer_config.y       = &bullet->position.y;
    renderer_config.sprite  = yx_spr_iris_bullet;
    renderer_config.z_group = 0;
    renderer_config.z_layer = 10000;
    k_object_add_sprite_renderer(obj_bullet, &renderer_config);

    k_object_add_step_callback(obj_bullet, bullet_touch_bubble);
}

/* endregion */

/* region [gun] */

static void shoot(struct k_object *object) {

    if (k_button_pressed(K_BUTTON_LEFT) || k_button_down(K_BUTTON_MIDDLE) || k_button_pressed(K_BUTTON_RIGHT)) {
        bullet_create(k_object_get_data(object));
    }
}

static void rotate_spr(struct k_object *object) {
    struct yx_obj_weapon *weapon = k_object_get_data(object);

    float mouse_x = (float)k_mouse_x();
    float mouse_y = (float)k_mouse_y();
    float weapon_x = weapon->position->x;
    float weapon_y = weapon->position->y;
    float angle = atanf((mouse_y - weapon_y) / (mouse_x - weapon_x)) / 3.14159265358979323846f * 180;

    if (weapon_x < mouse_x)
        k_sprite_renderer_flip_x(weapon->spr_rdr, 1);
    else
        k_sprite_renderer_flip_x(weapon->spr_rdr, 0);

    k_sprite_renderer_rotate(weapon->spr_rdr, angle);
}

struct yx_obj_weapon *yx_obj_weapon_create(const struct yx_obj_weapon_config *config) {
    struct k_object *object = k_object_create(sizeof(struct yx_obj_weapon));

    struct yx_obj_weapon *weapon = k_object_get_data(object);
    weapon->object = object;
    weapon->position = config->position;

    {
        struct k_sprite_renderer_config renderer_config;
        renderer_config.x       = &config->position->x;
        renderer_config.y       = &config->position->y;
        renderer_config.sprite  = yx_spr_iris_gun;
        renderer_config.z_group = 0;
        renderer_config.z_layer = (int)config->position->y + 1;
        weapon->spr_rdr = k_object_add_sprite_renderer(object, &renderer_config);
    }

    k_object_add_step_callback(object, rotate_spr);
    k_object_add_step_callback(object, shoot);

    return weapon;
}

void yx_obj_weapon_destroy(struct yx_obj_weapon *weapon) {

    if (NULL != weapon)
        k_object_destroy(weapon->object);
}

void yx_obj_weapon_set_z_index(struct yx_obj_weapon *weapon, int z_index) {
    k_sprite_renderer_set_z_layer(weapon->spr_rdr, z_index);
}

/* endregion */
