#include "../_internal.h"

static void bubble_del(struct k_object *object) {
    struct yx_obj_bubble *bubble = k_object_get_data(object);

    k_sprite_renderer_set_sprite(bubble->spr_rdr, yx_spr_bubble_del);
    k_sprite_renderer_set_loop(bubble->spr_rdr, 1);
    k_sprite_renderer_set_loop_callback(bubble->spr_rdr, k_object_destroy);
}

struct k_object *yx_bubble_create(float x, float y) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_bubble));

    struct yx_obj_bubble *bubble = k_object_get_data(object);
    bubble->x = x;
    bubble->y = y;

    struct k_sprite_renderer_config renderer_config;
    renderer_config.sprite  = yx_spr_bubble_new;
    renderer_config.z_index = 2;
    renderer_config.x       = &bubble->x;
    renderer_config.y       = &bubble->y;
    bubble->spr_rdr = k_object_add_sprite_renderer(object, &renderer_config);

    k_sprite_renderer_set_loop(bubble->spr_rdr, 1);
    k_sprite_renderer_set_loop_callback(bubble->spr_rdr, bubble_del);

    return object;
}
