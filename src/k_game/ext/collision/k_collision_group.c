#include "./_internal.h"

int k__collision_manager_init_group_map(struct k_collision_manager *manager) {
    size_t buckets_num = 32;
    struct k_hash_list *buckets = k_malloc(sizeof(struct k_hash_list) * buckets_num);
    if (buckets == NULL)
        return -1;

    k_int_map_init(&manager->group_map, buckets, buckets_num);
    return 0;
}

void k__collision_manager_fini_group_map(struct k_collision_manager *manager) {

    /* [?]
     * 如果不支持移除房间已挂载的组件管理器，那么管理器只会在销毁房间时被移除，本函数也只会在销毁房间时才被调用。
     * 销毁房间时，是先销毁所有对象，再移除管理器。而销毁对象时，碰撞盒组件在随之被销毁了。所以这里每个 group 都应该是空的。
     */

    struct k_collision_group *group;
    struct k_int_map *group_map = &manager->group_map;
    struct k_hash_list *bucket;
    for (k_int_map_for_each_bucket(group_map, bucket)) {
        struct k_hash_list_node *iter, *next;
        for (k_hash_list_for_each_s(bucket, iter, next)) {
            group = k_int_map_node_container_of(iter, struct k_collision_group, group_map_node);

            k__collision_manager_del_group(group);
        }
    }

    k_free(manager->group_map.buckets);
}

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

        new_group->cb_debug_draw = NULL;

        return new_group;
    }
}

void k__collision_manager_del_group(struct k_collision_group *group) {

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        k_object_del_component(box->component);
    }
    k_int_map_del(&group->group_map_node);

    if (NULL != group->cb_debug_draw)
        k_room_del_callback(group->cb_debug_draw);

    k_free(group);
}

struct k_collision_group *k__collision_find_or_add_group(int group_id) {

    struct k_component_manager *component_manager = k_get_room_component_manager(k__component_type_collision);
    if (NULL == component_manager)
        return NULL;

    struct k_collision_manager *manager = k_component_manager_get_data(component_manager);
    return k__collision_manager_find_or_add_group(manager, group_id);
}

struct k_collision_group *k__collision_find_group(int group_id) {

    struct k_component_manager *component_manager = k_get_room_component_manager(k__component_type_collision);
    if (NULL == component_manager)
        return NULL;

    struct k_collision_manager *manager = k_component_manager_get_data(component_manager);
    struct k_int_map_node *map_node = k_int_map_get(&manager->group_map, group_id);
    if (NULL == map_node)
        return NULL;

    struct k_collision_group *group = container_of(map_node, struct k_collision_group, group_map_node);
    return group;
}
