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
    struct k_collision_manager *manager = k_component_manager_get_data(component_manager);

    {
        /* [?]
         * 如果不支持移除房间已挂载的组件管理器，那么本函数只会在销毁房间时被调用
         * 销毁房间时，先销毁所有对象，在移除管理器。销毁对象时，碰撞盒在随之被销毁了。
         * 所以这里每个 group 都应该是空的。
         */

        struct k_collision_group *group;
        struct k_int_map *group_map = &manager->group_map;
        struct k_hash_list *bucket;
        struct k_hash_list_node *iter, *next;
        for (k_int_map_for_each_bucket(group_map, bucket)) {
            for (k_hash_list_for_each_s(bucket, iter, next)) {
                group = k_int_map_container_of(iter, struct k_collision_group, group_map_node);

                k__collision_manager_del_group(group);
            }
        }
    }

    k_free(manager->group_map.buckets);
}
