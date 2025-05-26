
#include "k_game/core/k_mem_alloc.h"
#include "k_game/core/k_object.h"

#include "./k_object_pool.h"
#include "./k_object.h"
#include "../room/k_room.h"

int k__object_pool_init(struct k_object_pool *pool) {

    k_list_init(&pool->object_list);

    size_t bucket_num = 64;
    struct k_hash_list *buckets = k__mem_alloc(sizeof(struct k_hash_list) * bucket_num);
    if (NULL == buckets)
        return -1;
    k_int_intrusive_map_init(&pool->id_map, buckets, bucket_num);

    return 0;
}

void k__object_pool_cleanup(struct k_object_pool *pool) {

    struct k_object *object;
    struct k_list *list = &pool->object_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        object = container_of(iter, struct k_object, pool_node);

        k_object_destroy(object);
    }
}

struct k_object *k__object_pool_acquire(struct k_object_pool *pool) {

    struct k_object *object = k__mem_alloc(sizeof(struct k_object)); /* TODO pool_alloc */
    if (NULL == object)
        return NULL;

    k_list_insert_tail(&pool->object_list, &object->pool_node.object_list_node);

    static int id_count = 0;
    id_count++;
    k_int_intrusive_map_add_directly(&pool->id_map, id_count, &object->pool_node.id_map_node);

    return object;
}

void k__object_pool_release(struct k_object *object) {
    k_list_remove(&object->pool_node.object_list_node);
    k_int_intrusive_map_remove(&object->pool_node.id_map_node);
    k__mem_free(object); /* TODO pool_free */
}

size_t k_object_get_id(struct k_object *object) {
    return object->pool_node.id_map_node.key;
}

struct k_object *k_object_find_by_id(size_t id) {

    struct k_int_intrusive_map_node *map_node = k_int_intrusive_map_get(&k__room_current->object_pool.id_map, (int)id);
    if (NULL == map_node)
        return NULL;

    struct k_object_pool_node *pool_node = container_of(map_node, struct k_object_pool_node, id_map_node);
    struct k_object *object = container_of(pool_node, struct k_object, pool_node);
    return object;
}
