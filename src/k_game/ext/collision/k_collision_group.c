#include "./_internal.h"

struct k_collision_group *k__collision_manager_find_or_add_group(struct k_collision_manager *manager, int group_id) {

    struct k_int_map_node *map_node = k_int_map_get(&manager->group_map, group_id);
    if (NULL != map_node) {
        struct k_collision_group *found_group = container_of(map_node, struct k_collision_group, group_map_node);
        return found_group;
    }
    else {
        struct k_collision_group *new_group = k_malloc(sizeof(struct k_collision_group));
        if (NULL == new_group)
            return NULL;

        k_int_map_add_directly(&manager->group_map, group_id, &new_group->group_map_node);
        k_list_init(&new_group->box_list);

        return new_group;
    }
}

void k__collision_manager_del_group(struct k_collision_group *group) {

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter_, *next_;
    for (k_list_for_each_s(list, iter_, next_)) {
        box = container_of(iter_, struct k_collision_box, box_list_node);

        k_object_del_component(box->component);
    }
}
