#include <limits.h>
#include <assert.h>

#include "./_internal.h"

/* region [collision_box_init] */

int k__collision_box_init(struct k_component *component, void *params) {
    struct k_collision_box *box = k_component_get_data(component);
    struct k_collision_box_config *box_config = params;

    int group_id;

    switch (box_config->type) {
        case K_COLLISION_RECTANGLE: {
            struct k_collision_rectangle_config *rect_config = box_config->config;
            box->type           = K_COLLISION_RECTANGLE;
            group_id            = rect_config->group_id;
            box->rect.position  = rect_config->position;
            box->rect.offset_x1 = rect_config->offset_x1;
            box->rect.offset_y1 = rect_config->offset_y1;
            box->rect.offset_x2 = rect_config->offset_x2;
            box->rect.offset_y2 = rect_config->offset_y2;
            break;
        }

        case K_COLLISION_CIRCLE: {
            struct k_collision_circle_config *circle_config = box_config->config;
            box->type             = K_COLLISION_CIRCLE;
            group_id              = circle_config->group_id;
            box->circle.position  = circle_config->position;
            box->circle.offset_cx = circle_config->offset_cx;
            box->circle.offset_cy = circle_config->offset_cy;
            box->circle.r         = circle_config->r;
            break;
        }

        default:
            assert(0);
            return -1;
    }

    struct k_collision_manager *manager = k_component_get_manager_data(component);
    if (NULL == manager)
        return -1;

    if (0 != k__collision_manager_add(manager, box, group_id)) // TODO 临时的 group_id 为 0
        return -1;

    k_component_add_draw_callback(component, k__collision_debug_draw, INT_MAX - 2);  // <- tmp
    return 0;
}

/* endregion */
