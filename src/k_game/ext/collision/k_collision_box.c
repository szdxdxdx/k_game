#include "./_internal.h"

/* region [collision_box_init] */

int k__collision_box_init(struct k_component *component, void *params) {
    struct k_collision_box_config *box_config = params;

    struct k_collision_manager *manager = k_component_get_manager_data(component);
    if (NULL == manager)
        return -1;

    struct k_collision_box *box = k_component_get_data(component);
    box->component = component;

    int group_id;

    switch (box_config->type) {

        case K_COLLISION_POINT: {
            const struct k_collision_point_config *point_config = box_config->config;
            group_id  = point_config->group_id;
            box->x    = point_config->x;
            box->y    = point_config->y;
            box->type = K_COLLISION_POINT;
            box->point.offset_x = point_config->offset_x;
            box->point.offset_y = point_config->offset_y;
            break;
        }
        case K_COLLISION_LINE: {
            const struct k_collision_line_config *line_config = box_config->config;
            group_id  = line_config->group_id;
            box->x    = line_config->x;
            box->y    = line_config->y;
            box->type = K_COLLISION_LINE;
            box->line.offset_x1 = line_config->offset_x1;
            box->line.offset_y1 = line_config->offset_y1;
            box->line.offset_x2 = line_config->offset_x2;
            box->line.offset_y2 = line_config->offset_y2;
            break;
        }
        case K_COLLISION_RECTANGLE: {
            const struct k_collision_rectangle_config *rect_config = box_config->config;
            group_id  = rect_config->group_id;
            box->x    = rect_config->x;
            box->y    = rect_config->y;
            box->type = K_COLLISION_RECTANGLE;
            box->rect.offset_x1 = rect_config->offset_x1;
            box->rect.offset_y1 = rect_config->offset_y1;
            box->rect.offset_x2 = rect_config->offset_x2;
            box->rect.offset_y2 = rect_config->offset_y2;
            break;
        }
        case K_COLLISION_CIRCLE: {
            const struct k_collision_circle_config *circle_config = box_config->config;
            group_id  = circle_config->group_id;
            box->x    = circle_config->x;
            box->y    = circle_config->y;
            box->type = K_COLLISION_CIRCLE;
            box->circle.offset_cx = circle_config->offset_cx;
            box->circle.offset_cy = circle_config->offset_cy;
            box->circle.r         = circle_config->r;
            break;
        }
    }

    struct k_collision_group *group = k__collision_manager_find_or_add_group(manager, group_id);
    if (NULL == group) {
        return -1;
    } else {
        k_list_add_tail(&group->box_list, &box->box_list_node);
    }

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
