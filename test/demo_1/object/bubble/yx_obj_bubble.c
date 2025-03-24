#include "../_internal.h"

static void bubble_dead(struct k_object *object, int timeout_diff) {
    yx_bubble_pop(object);
}

static void bubble_floating(struct k_object *object) {
    struct yx_obj_bubble *bubble = k_object_get_data(object);
    k_sprite_renderer_set_sprite(bubble->spr_rdr, yx_spr_bubble_float);
}

struct k_object *yx_bubble_create(float x, float y) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_bubble));
    struct yx_obj_bubble *bubble = k_object_get_data(object);
    bubble->position.x = x;
    bubble->position.y = y;

    // k_object_add_alarm_callback(object, bubble_dead, 2000 + rand() % 10000);

    {
        struct k_sprite_renderer_config config;
        config.x        = &bubble->position.x;
        config.y        = &bubble->position.y;
        config.sprite   = yx_spr_bubble_appear;
        config.z_group  = 0;
        config.z_layer  = (int)y;
        bubble->spr_rdr = k_object_add_sprite_renderer(object, &config);

        k_sprite_renderer_set_loop(bubble->spr_rdr, 1);
        k_sprite_renderer_set_loop_callback(bubble->spr_rdr, bubble_floating);
    }

    {
        struct k_collision_circle_config config;
        config.group_id  = YX_COLLISION_GROUP_BUBBLE;
        config.x         = &bubble->position.x;
        config.y         = &bubble->position.y;
        config.offset_cx = 0;
        config.offset_cy = -8;
        config.r         = 16;
        bubble->collision_box = k_object_add_collision_circle(object, &config);
    }

    return object;
}

void yx_bubble_pop(struct k_object *obj_bubble) {

    if (NULL == obj_bubble)
        return;

    struct yx_obj_bubble *bubble = k_object_get_data(obj_bubble);

    struct k_collision_box *box = bubble->collision_box;
    if (NULL == box)
        return;

    bubble->collision_box = NULL;
    k_object_del_collision_box(box);

    k_sprite_renderer_set_sprite(bubble->spr_rdr, yx_spr_bubble_pop);
    k_sprite_renderer_set_loop(bubble->spr_rdr, 1);
    k_sprite_renderer_set_loop_callback(bubble->spr_rdr, k_object_destroy);
}
