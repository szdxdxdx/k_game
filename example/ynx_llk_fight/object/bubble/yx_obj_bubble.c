
#define K_LOG_TAG "yx:object:bubble"
#include "k_log.h"

#include "object/bubble/yx_obj_bubble.h"

#include "sprite/yx_spr.h"

static void yx_obj_bubble_on_state_change_float(struct k_object *object) {
    struct yx_obj_bubble *bubble = k_object_get_data(object);

    k_sprite_renderer_set_sprite(bubble->spr_rdr, yx_spr_bubble_float);

    struct k_collision_rect_config config;
    config.group_id  = YX_COLLISION_GROUP_BUBBLE;
    config.x         = &bubble->x;
    config.y         = &bubble->y;
    config.offset_x1 = 8;
    config.offset_y1 = -6;
    config.offset_x2 = -8;
    config.offset_y2 = 10;
    bubble->collision_box = k_object_add_collision_rect(object, &config);
}

struct k_object *yx_obj_bubble_create(float x, float y) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_bubble));
    if (NULL == object)
        goto err;

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
        k_sprite_renderer_set_loop_callback(bubble->spr_rdr, yx_obj_bubble_on_state_change_float);
    }

    return object;

err:
    k_log_error("failed to create object: bubble");
    return NULL;
}

void yx_obj_bubble_pop(struct k_object *obj_bubble) {

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
