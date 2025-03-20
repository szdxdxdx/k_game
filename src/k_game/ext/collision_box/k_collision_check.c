#include "./k_collision.h"

struct k_collision_box *k_collision_check_rectangle(int group_id, float x1, float y1, float x2, float y2) {

    struct k_component_manager *component_manager = k_room_get_component_manager(k__component_type_collision_box);
    if (NULL == component_manager)
        return NULL;

    struct k_collision_manager *manager = k_component_manager_get_data(component_manager);
    struct k_int_map_node *map_node = k_int_map_get(&manager->group_map, group_id);
    if (NULL == map_node)
        return NULL;

    struct k_collision_group *group = container_of(map_node, struct k_collision_group, map_node);

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, list_node);

        switch (box->box_type) {
            case K_COLLISION_BOX_RECTANGLE: {
                struct k_collision_rectangle *rect = &box->rectangle;

                float x3 = *(rect->x) + rect->offset_x1;
                float y3 = *(rect->y) + rect->offset_y1;
                float x4 = *(rect->x) + rect->offset_x2;
                float y4 = *(rect->y) + rect->offset_y2;
                if (   ((x1 < x4) != (x2 < x3) && (y1 < y4) != (y2 < y3))
                    || ((x1 < x3) != (x2 < x4) && (y1 < y3) != (y2 < y4))
                ) {
                    return box;
                }

                break;
            }
            case K_COLLISION_BOX_CIRCLE: {
                break;
            }
        }
    }

    return NULL;
}

struct k_collision_box *k_collision_check_circle(int group_id, float x, float y, float r) {

    return NULL;
}

