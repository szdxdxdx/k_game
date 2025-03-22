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

    /* TODO */
}

static struct k_collision_group *find_or_create_group(struct k_collision_manager *manager, int group_id) {

    struct k_int_map_node *map_node = k_int_map_get(&manager->group_map, group_id);
    if (NULL != map_node) {
        struct k_collision_group *found_group = container_of(map_node, struct k_collision_group, map_node);
        return found_group;
    }
    else {
        struct k_collision_group *new_group = k_malloc(sizeof(struct k_collision_group));
        if (NULL == new_group)
            return NULL;

        k_int_map_add_directly(&manager->group_map, group_id, &new_group->map_node);
        k_list_init(&new_group->box_list);

        return new_group;
    }
}

int k__collision_manager_add(struct k_collision_manager *manager, struct k_collision_box *box, int group_id) {

    struct k_collision_group *group = find_or_create_group(manager, group_id);
    if (NULL == group)
        return -1;

    k_list_add_tail(&group->box_list, &box->list_node);
    return 0;
}
