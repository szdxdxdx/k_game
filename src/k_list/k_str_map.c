#include <assert.h>

#include "k_str_map.h"

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

struct k_str_map *k_str_map_create(const struct k_str_map_config *config) {
    assert(NULL != config);
    assert(NULL != config->fn_malloc);
    assert(NULL != config->fn_free);

    struct k_str_map *map = config->fn_malloc(sizeof(struct k_str_map));
    if (NULL == map)
        return NULL;

    size_t buckets_num = 8;
    struct k_hash_list *buckets = config->fn_malloc(sizeof(struct k_hash_list) * buckets_num);
    if (NULL == buckets) {
        config->fn_free(map);
        return NULL;
    }

    map->fn_malloc = config->fn_malloc;
    map->fn_free   = config->fn_free;
    map->size      = 0;
    k_str_hash_map_init(&map->map, buckets, buckets_num);

    return map;
}

void k_str_map_destroy(struct k_str_map *map) {

    struct k_hash_list *buckets;
    for (k_str_hash_map_for_each_bucket((&map->map), buckets)) {

    }
}

void *k_str_map_put(struct k_str_map *map, const char *key, size_t value_size) {

}

void k_str_map_del(struct k_str_map *map, const char *key) {

}

void *k_str_map_get(struct k_str_map *map, const char *key) {

}
