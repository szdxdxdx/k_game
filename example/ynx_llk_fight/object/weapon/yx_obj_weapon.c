#include "k_game.h"

#include "utils/yx_math.h"
#include "sprite/yx_spr.h"

#include "object/weapon/yx_obj_weapon.h"
#include "object/bubble/yx_obj_bubble.h"

#if 0

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
            yx_obj_bubble_pop(k_collision_box_get_object(box));
    }
}

static void bullet_move(struct k_object *object) {
    struct yx_obj_bullet *bullet = k_object_get_data(object);

    float delta = k_time_get_step_delta();
    bullet->x += bullet->velocity_x * delta;
    bullet->y += bullet->velocity_y * delta;

    if (   bullet->x < 0 || bullet->x > k_room_get_w()
        || bullet->y < 0 || bullet->y > k_room_get_h()
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

/* region [apple] */

struct yx_obj_apple {

    float velocity_x;
    float velocity_y;
    float x;
    float y;

    float angle;

    struct k_sprite_renderer *spr_rdr;
};

static void yx_obj_apple_on_step_touch_bubble(struct k_object *object) {
    struct yx_obj_apple *apple = k_object_get_data(object);
    float x1 = apple->x - 4;
    float y1 = apple->y - 4;
    float x2 = apple->x + 4;
    float y2 = apple->y + 4; /* 指定一个以苹果对象为中心的矩形区域 */
    struct k_collision_box *box = k_collision_check_rect(0, x1, y1, x2, y2); /* 主动查询与该区域相交的碰撞盒 */
    if (NULL != box) /* 本示例中，该碰撞盒是绑定绑定在泡泡对象上的，所以直接获取该碰撞盒绑定的泡泡对象，然后戳破泡泡 */
        yx_obj_bubble_pop(k_collision_box_get_object(box));
}

static void yx_obj_apple_on_step_move(struct k_object *object) {
    struct yx_obj_apple *apple = k_object_get_data(object);

    float delta = k_time_get_step_delta();
    apple->x += apple->velocity_x * delta;
    apple->y += apple->velocity_y * delta;

    if (   apple->x < 0 || apple->x > k_room_get_w()
        || apple->y < 0 || apple->y > k_room_get_h()
    ) {
        k_object_destroy(object);
        return;
    }

    apple->angle += delta * 300.0f;
    k_sprite_renderer_rotate(apple->spr_rdr, apple->angle);
}

/* 创建苹果 */
static void yx_obj_apple_create(struct yx_obj_weapon *weapon) {

    struct k_object *obj_apple = k_object_create(sizeof(struct yx_obj_apple));
    struct yx_obj_apple *apple = k_object_get_data(obj_apple);

    float mouse_x = k_mouse_x();
    float mouse_y = k_mouse_y();
    float weapon_x = weapon->x;
    float weapon_y = weapon->y;
    float cos_angle;
    float sin_angle;
    yx_calc_vector_direction(weapon_x, weapon_y, mouse_x, mouse_y, &cos_angle, &sin_angle);

    float speed = 300.0f;

    apple->velocity_x = cos_angle * speed;
    apple->velocity_y = sin_angle * speed;

    apple->x = weapon->x + cos_angle * 20;
    apple->y = weapon->y + sin_angle * 20;

    k_object_add_step_callback(obj_apple, yx_obj_apple_on_step_move);

    struct k_sprite_renderer_config renderer_config;
    renderer_config.x       = &apple->x;
    renderer_config.y       = &apple->y;
    renderer_config.sprite  = yx_spr_iris_apple;
    renderer_config.z_group = 0;
    renderer_config.z_layer = 10000;
    apple->spr_rdr = k_object_add_sprite_renderer(obj_apple, &renderer_config);

    k_object_add_step_callback(obj_apple, yx_obj_apple_on_step_touch_bubble);
}

/* endregion */

/* region [gun] */

static void yx_obj_weapon_on_step_shoot(struct k_object *object) {

    if (k_mouse_button_down_or_held(K_BUTTON_LEFT) || k_mouse_button_down(K_BUTTON_RIGHT)) {
        struct yx_obj_weapon *weapon = k_object_get_data(object);
        yx_obj_apple_create(weapon);
    }
}

static void yx_obj_weapon_on_end_step(struct k_object *object) {
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

    k_sprite_renderer_set_z_layer(weapon->spr_rdr, (int)weapon->y);
}

static void mouse_drag(struct k_object *object) {
    struct yx_obj_weapon *weapon = k_object_get_data(object);

    if (k_key_down_or_held(K_KEY_LEFT_SHIFT)) {
        float x = k_mouse_x();
        float y = k_mouse_y();
        k_position_set_world_position(weapon->position, x, y);
    }
}

struct yx_obj_weapon *yx_obj_weapon_create(const struct yx_obj_weapon_config *config) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_weapon));

    k_object_add_step_callback(object, yx_obj_weapon_on_step_shoot);
    k_object_add_end_step_callback(object, yx_obj_weapon_on_end_step);
    // k_object_add_step_callback(object, mouse_drag);

    struct yx_obj_weapon *weapon = k_object_get_data(object);

    weapon->object = object;

    {
        struct k_position_config position_config;
        position_config.world_x = &weapon->x;
        position_config.world_y = &weapon->y;
        position_config.parent  = config->parent;
        position_config.local_x = 0;
        position_config.local_y = 1;
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

void yx_obj_weapon_destroy(struct yx_obj_weapon *weapon) {

    if (NULL != weapon)
        k_object_destroy(weapon->object);
}

/* endregion */

#endif
