#include <string.h>
#include <assert.h>

#include "k_str_intrusive_map.h"

static inline size_t str_hash(const char *str) {

    size_t hash = 5381;
    for (; '\0' != *str; str++) {
        hash = hash * 33 + *str;
    }

    return hash;
}

static inline struct k_str_intrusive_map_node *find(struct k_hash_list *list, const char *key, size_t hash) {

    struct k_str_intrusive_map_node *map_node;
    struct k_hash_list_node *iter;
    for (k_hash_list_for_each(list, iter)) {
        map_node = container_of(iter, struct k_str_intrusive_map_node, node_link);

        if (map_node->hash == hash) {
            if (strcmp(map_node->key, key) == 0)
                return map_node;
        }
    }

    return NULL;
}

void k_str_intrusive_map_init(struct k_str_intrusive_map *map, struct k_hash_list *buckets, size_t buckets_num) {
    assert(NULL != map);
    assert(NULL != buckets);
    assert(1 <= buckets_num);

    k_hash_list_init_all(buckets, buckets_num);

    map->buckets = buckets;
    map->buckets_num = buckets_num;
}

int k_str_intrusive_map_add_if_absent(struct k_str_intrusive_map *map, const char *key, struct k_str_intrusive_map_node *node) {
    assert(NULL != map);
    assert(NULL != key);
    assert(NULL != node);

    size_t hash = str_hash(key);

    struct k_hash_list *list = &(map->buckets[hash % map->buckets_num]);
    if (NULL != find(list, key, hash))
        return -1;

    node->key = key;
    node->hash = hash;
    k_hash_list_add(list, &node->node_link);

    return 0;
}

void k_str_intrusive_map_add_directly(struct k_str_intrusive_map *map, const char *key, struct k_str_intrusive_map_node *node) {
    assert(NULL != map);
    assert(NULL != key);
    assert(NULL != node);

    size_t hash = str_hash(key);
    struct k_hash_list *list = &(map->buckets[hash % map->buckets_num]);

    node->key = key;
    node->hash = hash;
    k_hash_list_add(list, &node->node_link);
}

struct k_str_intrusive_map_node *k_str_intrusive_map_get(struct k_str_intrusive_map *map, const char *key) {
    assert(NULL != map);
    assert(NULL != key);

    size_t hash = str_hash(key);
    struct k_hash_list *list = &(map->buckets[hash % map->buckets_num]);
    return find(list, key, hash);
}

void k_str_intrusive_map_del(struct k_str_intrusive_map_node *node) {
    assert(NULL != node);
    k_hash_list_del(&node->node_link);
}

struct k_hash_list *k_str_intrusive_map_rehash(struct k_str_intrusive_map *map, struct k_hash_list *new_buckets, size_t new_buckets_num) {
    assert(NULL != map);
    assert(NULL != new_buckets);
    assert(1 <= new_buckets_num);

    k_hash_list_init_all(new_buckets, new_buckets_num);

    struct k_hash_list *old_buckets = map->buckets;
    size_t old_buckets_num = map->buckets_num;

    struct k_hash_list *old_list = old_buckets;
    for (; old_list < old_buckets + old_buckets_num; old_list++) {

        struct k_str_intrusive_map_node *map_node;
        struct k_hash_list_node *iter, *next;
        for (k_hash_list_for_each_s(old_list, iter, next)) {
            map_node = container_of(iter, struct k_str_intrusive_map_node, node_link);

            struct k_hash_list *new_list = &(new_buckets[map_node->hash % new_buckets_num]);
            k_hash_list_add(new_list, &map_node->node_link);
        }
    }

    map->buckets = new_buckets;
    map->buckets_num  = new_buckets_num;

    return old_buckets;
}
