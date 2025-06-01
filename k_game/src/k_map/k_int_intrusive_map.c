#include <assert.h>

#include "k_int_intrusive_map.h"

void k_int_intrusive_map_init(struct k_int_intrusive_map *map, struct k_hash_list *buckets, size_t buckets_num) {
    assert(NULL != map);
    assert(NULL != buckets);
    assert(1 <= buckets_num);

    k_hash_list_init_all(buckets, buckets_num);

    map->buckets = buckets;
    map->buckets_num = buckets_num;
}

static inline struct k_int_intrusive_map_node *find(struct k_hash_list *bucket, int key) {

    struct k_int_intrusive_map_node *map_node;
    struct k_hash_list_node *iter;
    for (k_hash_list_for_each(bucket, iter)) {
        map_node = container_of(iter, struct k_int_intrusive_map_node, node_link);

        if (map_node->key == key)
            return map_node;
    }

    return NULL;
}

int k_int_intrusive_map_add_if_absent(struct k_int_intrusive_map *map, int key, struct k_int_intrusive_map_node *node) {
    assert(NULL != map);
    assert(NULL != node);

    size_t hash = (size_t)key;

    struct k_hash_list *bucket = &(map->buckets[hash % map->buckets_num]);
    if (NULL != find(bucket, key))
        return -1;

    node->key = key;
    k_hash_list_add(bucket, &node->node_link);

    return 0;
}

void k_int_intrusive_map_add_directly(struct k_int_intrusive_map *map, int key, struct k_int_intrusive_map_node *node) {
    assert(NULL != map);
    assert(NULL != node);

    size_t hash = (size_t)key;

    struct k_hash_list *list = &(map->buckets[hash % map->buckets_num]);

    node->key = key;
    k_hash_list_add(list, &node->node_link);
}

struct k_int_intrusive_map_node *k_int_intrusive_map_get(struct k_int_intrusive_map *map, int key) {
    assert(NULL != map);

    size_t hash = (size_t)key;

    struct k_hash_list *bucket = &(map->buckets[hash % map->buckets_num]);
    return find(bucket, key);
}

void k_int_intrusive_map_remove(struct k_int_intrusive_map_node *node) {
    assert(NULL != node);
    k_hash_list_remove(&node->node_link);
}

struct k_hash_list *k_int_intrusive_map_rehash(struct k_int_intrusive_map *map, struct k_hash_list *new_buckets, size_t new_buckets_num) {
    assert(NULL != map);
    assert(NULL != new_buckets);
    assert(1 <= new_buckets_num);

    k_hash_list_init_all(new_buckets, new_buckets_num);

    struct k_hash_list *old_buckets = map->buckets;
    size_t old_buckets_num = map->buckets_num;

    struct k_hash_list *old_list = old_buckets;
    for (; old_list < old_buckets + old_buckets_num; old_list++) {

        struct k_int_intrusive_map_node *map_node;
        struct k_hash_list_node *iter, *next;
        for (k_hash_list_for_each_s(old_list, iter, next)) {
            map_node = container_of(iter, struct k_int_intrusive_map_node, node_link);

            size_t hash = (size_t)(map_node->key);

            struct k_hash_list *new_list = &(new_buckets[hash % new_buckets_num]);
            k_hash_list_add(new_list, &map_node->node_link);
        }
    }

    map->buckets = new_buckets;
    map->buckets_num  = new_buckets_num;

    return old_buckets;
}
