#include "k_game.h"

#include "./my_object.h"

#include "../sprite/_public.h"

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

    return object;
}
