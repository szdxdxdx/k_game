#include <limits.h>
#include <assert.h>

#include "./_internal.h"

/* region [collision_box_init] */

int k__collision_box_init(struct k_component *component, void *params) {

    struct k_collision_manager *manager = k_component_get_manager_data(component);
    if (NULL == manager)
        return -1;

    struct k_collision_box *box = k_component_get_data(component);
    union k_collision_box_config *box_config = params;

    switch (box_config->box_type) {
        case K_COLLISION_BOX_RECTANGLE: {
            struct k_collision_rectangle_config *config = &box_config->rectangle;
            struct k_collision_rectangle *rect = &box->rectangle;

            rect->box_type  = config->box_type;
            rect->position  = config->position;
            rect->offset_x1 = config->offset_x1;
            rect->offset_y1 = config->offset_y1;
            rect->offset_x2 = config->offset_x2;
            rect->offset_y2 = config->offset_y2;
            break;
        }

        case K_COLLISION_BOX_CIRCLE: {
            struct k_collision_circle_config *config = &box_config->circle;
            struct k_collision_circle *circle = &box->circle;

            circle->box_type  = config->box_type;
            circle->position  = config->position;
            circle->offset_cx = config->offset_cx;
            circle->offset_cy = config->offset_cy;
            circle->r         = config->r;
            break;
        }

        default:
            assert(0);
            return -1;
    }

    if (0 != k__collision_manager_add(manager, box, 0)) // TODO 临时的 group_id 为 0
        return -1;

    k_component_add_draw_callback(component, k__collision_debug_draw, INT_MAX - 2);  // <- tmp
    return 0;
}

/* endregion */
