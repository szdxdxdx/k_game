#include "k_game.h"

#include "./my_object.h"

#include "../sprite/_public.h"

void my_bubble_destroy(void *data) {
    struct k_object *object = data;
    k_object_destroy(object);
}

void my_bubble_del(void *data) {
    struct k_object *object = data;
    struct my_bubble *bubble = k_object_get_data(object);

    k_sprite_renderer_set_sprite(bubble->spr_rdr, my_spr_bubble_del);
    k_sprite_renderer_set_loop(bubble->spr_rdr, 1);
    k_sprite_renderer_set_loop_callback(bubble->spr_rdr, my_bubble_destroy, object);
}

struct k_object *my_bubble_create(float x, float y) {

    struct k_object *object = k_object_create(sizeof(struct my_bubble));

    struct my_bubble *bubble = k_object_get_data(object);
    bubble->x = x;
    bubble->y = y;

    struct k_sprite_renderer_config renderer_config;
    renderer_config.sprite  = my_spr_bubble_new;
    renderer_config.z_index = 2;
    renderer_config.x       = &bubble->x;
    renderer_config.y       = &bubble->y;
    bubble->spr_rdr = k_object_add_sprite_renderer(object, &renderer_config);

    k_sprite_renderer_set_loop(bubble->spr_rdr, 1);
    k_sprite_renderer_set_loop_callback(bubble->spr_rdr, my_bubble_del, object);

    return object;
}
