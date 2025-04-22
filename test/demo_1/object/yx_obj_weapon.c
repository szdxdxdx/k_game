#include "./yx_obj.h"

#include "../utils/yx_math.h"
#include "../sprite/yx_spr.h"

/* region [bullet] */

struct yx_obj_bullet {

    float velocity_x;
    float velocity_y;
    float x;
    float y;
};

static void bullet_touch_bubble(struct k_object *object) {
    struct yx_obj_bullet *bullet = k_object_get_data(object);

    {
        float padding = 2;
        float x1 = bullet->x - padding;
        float y1 = bullet->y - padding;
        float x2 = bullet->x + padding;
        float y2 = bullet->y + padding;
        struct k_collision_box *box = k_collision_check_rect(YX_COLLISION_GROUP_BUBBLE, x1, y1, x2, y2);
        if (NULL != box)
            yx_pop_bubble(k_collision_box_get_object(box));
    }
}

static void bullet_move(struct k_object *object) {
    struct yx_obj_bullet *bullet = k_object_get_data(object);

    float delta = k_get_step_delta();
    bullet->x += bullet->velocity_x * delta;
    bullet->y += bullet->velocity_y * delta;

    if (   bullet->x < 0 || bullet->x > k_room_get_width()
        || bullet->y < 0 || bullet->y > k_room_get_height()
    ) {
        k_object_destroy(object);
    }
}

static void bullet_create(struct yx_obj_weapon *weapon) {
    struct k_object *obj_bullet = k_object_create(sizeof(struct yx_obj_bullet));
    struct yx_obj_bullet *bullet = k_object_get_data(obj_bullet);

    float mouse_x = k_mouse_x();
    float mouse_y = k_mouse_y();
    float weapon_x = weapon->x;
    float weapon_y = weapon->y;

    float cos_angle;
    float sin_angle;
    yx_calc_vector_direction(weapon_x, weapon_y, mouse_x, mouse_y, &cos_angle, &sin_angle);

    float speed = 300.0f;

    bullet->velocity_x = cos_angle * speed;
    bullet->velocity_y = sin_angle * speed;

    bullet->x = weapon->x + cos_angle * 20;
    bullet->y = weapon->y + sin_angle * 20;

    k_object_add_step_callback(obj_bullet, bullet_move);

    struct k_sprite_renderer_config renderer_config;
    renderer_config.x       = &bullet->x;
    renderer_config.y       = &bullet->y;
    renderer_config.sprite  = yx_spr_iris_bullet;
    renderer_config.z_group = 0;
    renderer_config.z_layer = 10000;
    k_object_add_sprite_renderer(obj_bullet, &renderer_config);

    k_object_add_step_callback(obj_bullet, bullet_touch_bubble);
}

/* endregion */

/* region [gun] */

static void shoot(struct k_object *object) {

    if (k_mouse_button_down(K_BUTTON_LEFT) || k_mouse_button_pressed(K_BUTTON_RIGHT)) {
        struct yx_obj_weapon *weapon = k_object_get_data(object);
        bullet_create(weapon);
    }
}

static void draw_weapon(struct k_object *object) {
    struct yx_obj_weapon *weapon = k_object_get_data(object);

    float mouse_x = k_mouse_x();
    float weapon_x = weapon->x;

    if (mouse_x != weapon_x) {
        float angle = atanf((k_mouse_y() - weapon->y) / (mouse_x - weapon_x));
        angle *= 180.0f / 3.1415926f;

        k_sprite_renderer_rotate(weapon->spr_rdr, angle);

        if (weapon_x < mouse_x) {
            k_sprite_renderer_flip_x(weapon->spr_rdr, 1);
        }
        else {
            k_sprite_renderer_flip_x(weapon->spr_rdr, 0);
        }
    }
}

void mouse_drag(struct k_object *object) {
    struct yx_obj_weapon *weapon = k_object_get_data(object);

    if (k_key_down(K_KEY_LEFT_SHIFT)) {
        float x = k_mouse_x();
        float y = k_mouse_y();
        k_position_set_world_position(weapon->position, x, y);
    }
}

void after_move(struct k_object *object) {
    struct yx_obj_weapon *weapon = k_object_get_data(object);
    k_sprite_renderer_set_z_layer(weapon->spr_rdr, (int)weapon->y);
}

struct yx_obj_weapon *yx_create_weapon(const struct yx_obj_weapon_config *config) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_weapon));

    k_object_add_step_callback(object, draw_weapon);
    k_object_add_step_callback(object, shoot);
    k_object_add_step_callback(object, mouse_drag);
    k_object_add_step_end_callback(object, after_move);

    struct yx_obj_weapon *weapon = k_object_get_data(object);

    weapon->object = object;

    {
        struct k_position_config position_config;
        position_config.world_x      = &weapon->x;
        position_config.world_y      = &weapon->y;
        position_config.parent = config->parent;
        position_config.local_x  = 0;
        position_config.local_y  = 1;
        weapon->position = k_object_add_position(object, &position_config);
    }

    {
        struct k_sprite_renderer_config renderer_config;
        renderer_config.x       = &weapon->x;
        renderer_config.y       = &weapon->y;
        renderer_config.sprite  = yx_spr_iris_gun;
        renderer_config.z_group = 0;
        renderer_config.z_layer = 0;
        weapon->spr_rdr = k_object_add_sprite_renderer(object, &renderer_config);
    }

    return weapon;
}

void yx_destroy_weapon(struct yx_obj_weapon *weapon) {

    if (NULL != weapon)
        k_object_destroy(weapon->object);
}

/* endregion */
