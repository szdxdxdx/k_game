#include <assert.h>
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
        struct k_collision_group *group;
        struct k_int_map *group_map = &manager->group_map;
        struct k_hash_list *bucket;
        struct k_hash_list_node *iter, *next;
        for (k_int_map_for_each_bucket(group_map, bucket)) {
            for (k_hash_list_for_each_s(bucket, iter, next)) {
                group = k_int_map_container_of(iter, struct k_collision_group, group_map_node);

                struct k_collision_box *box;
                struct k_list *list = &group->box_list;

                /* 销毁房间对象的时候，碰撞盒在随之被销毁了，所以每个 group 都应该是空的 */
                assert( k_list_is_empty(list) );

                struct k_list_node *iter_, *next_;
                for (k_list_for_each_s(list, iter_, next_)) {
                    box = container_of(iter_, struct k_collision_box, box_list_node);

                    k_object_del_component(box->component);
                }
            }
        }
    }

    k_free(manager->group_map.buckets);
}
