#include "./_internal.h"

int k__collision_manager_init(struct k_component_manager *component_manager, void *params) {
    (void)params;

    struct k_collision_manager *manager = k_component_manager_get_data(component_manager);

    size_t buckets_num = 32;
    struct k_hash_list *buckets = k_malloc(sizeof(struct k_hash_list) * buckets_num);
    if (buckets == NULL)
        return -1;

    k_int_map_init(&manager->group_map, buckets, buckets_num);
    return 0;
}

void k__collision_manager_fini(struct k_component_manager *component_manager) {

}

int k__collision_manager_add(struct k_collision_manager *manager, struct k_collision_box *box, int group_idx) {

    struct k_collision_group *group;

    struct k_int_map_node *map_node = k_int_map_get(&manager->group_map, group_idx);
    if (NULL != map_node) {
        group = container_of(map_node, struct k_collision_group, map_node);
    }
    else {
        group = k_malloc(sizeof(struct k_collision_group));
        if (NULL == group)
            return -1;

        k_int_map_add_directly(&manager->group_map, group_idx, &group->map_node);

        k_list_init(&group->box_list);
    }

    k_list_add_tail(&group->box_list, &box->list_node);
    return 0;
}
