#include "../_internal.h"

static void bubble_del(struct k_object *object) {
    struct yx_obj_bubble *bubble = k_object_get_data(object);

    k_sprite_renderer_set_sprite(bubble->spr_rdr, yx_spr_bubble_popped);
    k_sprite_renderer_set_loop(bubble->spr_rdr, 1);
    k_sprite_renderer_set_loop_callback(bubble->spr_rdr, k_object_destroy);
}

static void bubble_floating(struct k_object *object) {
    struct yx_obj_bubble *bubble = k_object_get_data(object);
    k_sprite_renderer_set_sprite(bubble->spr_rdr, yx_spr_bubble_floating);
}

struct k_object *yx_bubble_create(float x, float y) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_bubble));
    struct yx_obj_bubble *bubble = k_object_get_data(object);
    bubble->position.x = x;
    bubble->position.y = y;

    {
        struct k_sprite_renderer_config config;
        config.position = &bubble->position;
        config.sprite   = yx_spr_bubble_appearing;
        config.z_index  = (int)y;
        bubble->spr_rdr = k_object_add_sprite_renderer(object, &config);
        k_sprite_renderer_set_loop(bubble->spr_rdr, 1);
        k_sprite_renderer_set_loop_callback(bubble->spr_rdr, bubble_floating);
    }

    {
        struct k_collision_circle_config config;
        config.group_id  = YX_COLLISION_GROUP_BUBBLE;
        config.position  = &bubble->position;
        config.offset_cx = 0;
        config.offset_cy = 0;
        config.r         = 16;
        k_object_add_collision_circle(object, &config);
    }

    return object;
}
