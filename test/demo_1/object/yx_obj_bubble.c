#include "./yx_all_object.h"
#include "../sprite/yx_all_sprite.h"

static void bubble_set_state_float(struct k_object *object) {
    struct yx_obj_bubble *bubble = k_object_get_data(object);
    k_sprite_renderer_set_sprite(bubble->spr_rdr, yx_spr_bubble_float);
    k_sprite_renderer_rotate(bubble->spr_rdr, 45.0f);

    {
        struct k_collision_line_config config;
        config.group_id  = YX_COLLISION_GROUP_BUBBLE;
        config.x         = &bubble->x;
        config.y         = &bubble->y;
        config.offset_x1 = 8;
        config.offset_y1 = -6;
        config.offset_x2 = -8;
        config.offset_y2 = 10;
        bubble->collision_box = k_object_add_collision_line(object, &config);
    }
}

struct k_object *yx_bubble_create(float x, float y) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_bubble));
    struct yx_obj_bubble *bubble = k_object_get_data(object);
    bubble->x = x;
    bubble->y = y;

    {
        struct k_sprite_renderer_config config;
        config.x        = &bubble->x;
        config.y        = &bubble->y;
        config.sprite   = yx_spr_bubble_appear;
        config.z_group  = 0;
        config.z_layer  = (int)y;
        bubble->spr_rdr = k_object_add_sprite_renderer(object, &config);

        k_sprite_renderer_set_loop_count(bubble->spr_rdr, 1);
        k_sprite_renderer_set_loop_callback(bubble->spr_rdr, bubble_set_state_float);

        k_sprite_renderer_set_debug(bubble->spr_rdr, 1);
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
    k_sprite_renderer_set_loop_count(bubble->spr_rdr, 1);
    k_sprite_renderer_set_loop_callback(bubble->spr_rdr, k_object_destroy);
}
