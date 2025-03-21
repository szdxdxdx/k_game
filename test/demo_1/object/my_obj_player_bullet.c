#include "./my_object.h"

#include "../sprite/_public.h"

static void bullet_step(struct k_object *object) {
    struct my_player_bullet *bullet = k_object_get_data(object);
    struct k_room *room = k_get_current_room();

    float delta = k_get_step_delta();
    float x_add = bullet->speed * delta;

    struct k_collision_box *box = k_collision_check_rectangle(0, bullet->x, bullet->y - 2.0f, bullet->x + x_add, bullet->y + 2.0f);
    if (NULL != box) {
        k_object_destroy(object);
        return;
    }

    bullet->x += x_add;

    if (bullet->x < 20.0f || bullet->x > (float)k_room_get_width() - 20.f) {
        k_object_destroy(object);
    }
}

struct k_object *my_player_bullet_create(float x, float y, int face) {
    struct k_room *room = k_get_current_room();

    struct k_object *object = k_object_create(sizeof(struct my_player_bullet));
    struct my_player_bullet *bullet = k_object_get_data(object);
    bullet->x = x;
    bullet->y = y;
    bullet->speed = (face >= 0 ? 200.0f : -200.0f);

    k_object_add_step_callback(object, bullet_step);

    struct k_sprite_renderer_config config;
    config.sprite  = my_spr_player_bullet;
    config.z_index = 10;
    config.x       = &bullet->x;
    config.y       = &bullet->y;
    struct k_sprite_renderer *rdr = k_object_add_sprite_renderer(object, &config);
    if (bullet->speed < 0)
        k_sprite_renderer_flip_x(rdr, 1);

    return object;
}
