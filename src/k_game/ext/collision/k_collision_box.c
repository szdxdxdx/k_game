#include <assert.h>

#include "./_internal.h"

/* region [collision_box_init] */

int k__collision_box_init(struct k_component *component, void *params) {
    struct k_collision_box_config *box_config = params;

    struct k_collision_box *box = k_component_get_data(component);
    box->component = component;

    int group_id;

    switch (box_config->type) {
        case K_COLLISION_RECTANGLE: {
            const struct k_collision_rectangle_config *rect_config = box_config->config;
            box->type           = K_COLLISION_RECTANGLE;
            group_id            = rect_config->group_id;
            box->rect.x         = rect_config->x;
            box->rect.y         = rect_config->y;
            box->rect.offset_x1 = rect_config->offset_x1;
            box->rect.offset_y1 = rect_config->offset_y1;
            box->rect.offset_x2 = rect_config->offset_x2;
            box->rect.offset_y2 = rect_config->offset_y2;
            break;
        }

        case K_COLLISION_CIRCLE: {
            const struct k_collision_circle_config *circle_config = box_config->config;
            box->type             = K_COLLISION_CIRCLE;
            group_id              = circle_config->group_id;
            box->circle.x         = circle_config->x;
            box->circle.y         = circle_config->y;
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

    struct k_collision_group *group = k__collision_manager_find_or_add_group(manager, group_id);
    if (NULL == group)
        return -1;

    k_list_add_tail(&group->box_list, &box->box_list_node);
    return 0;
}

void k__collision_box_fini(struct k_component *component) {
    struct k_collision_box *box = k_component_get_data(component);

    k_list_del(&box->box_list_node);
}

/* endregion */

/* region [collision_box_get] */

struct k_object *k_collision_box_get_object(struct k_collision_box *box) {
    return k_component_get_object(box->component);
}

/* endregion */
