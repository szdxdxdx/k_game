#include "./_internal.h"

/* region [collision_manager] */

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

/* endregion */

int k__collision_manager_add(struct k_collision_manager *manager, struct k_collision_box *box, int group_id) {

    struct k_collision_group *group = k__collision_group_find_or_create(manager, group_id);
    if (NULL == group)
        return -1;

    k_list_add_tail(&group->box_list, &box->box_list_node);
    return 0;
}
