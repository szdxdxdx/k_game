#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#include "k_str_map.h"

struct k_str_map_node {
    struct k_str_hash_map_node hash_map_node;
};

static void *get_val(struct k_str_map_node *node) {
    return (void *)((char *)node + sizeof(struct k_str_map_node));
}

static struct k_str_map_options default_options = {
    .fn_malloc = malloc,
    .fn_free   = free,
};

struct k_str_map *k_str_map_create(const struct k_str_map_options *options) {

    if (NULL == options) {
        options = &default_options;
    }

    struct k_str_map *map = options->fn_malloc(sizeof(struct k_str_map));
    if (NULL == map)
        return NULL;

    size_t buckets_num = 17;
    struct k_hash_list *buckets = options->fn_malloc(sizeof(struct k_hash_list) * buckets_num);
    if (NULL == buckets) {
        options->fn_free(map);
        return NULL;
    }

    map->fn_malloc = options->fn_malloc;
    map->fn_free   = options->fn_free;
    map->size      = 0;
    map->rehash_threshold = 29;
    k_str_hash_map_init(&map->hash_map, buckets, buckets_num);

    return map;
}

void k_str_map_destroy(struct k_str_map *map) {

    if (NULL == map)
        return;

    struct k_str_map_node *map_node;
    struct k_str_hash_map *hash_map = &map->hash_map;
    struct k_hash_list *buckets;
    for (k_str_hash_map_for_each_bucket(hash_map, buckets)) {
        struct k_hash_list_node *iter, *next;
        for (k_hash_list_for_each_s(buckets, iter, next)) {
            map_node = k_str_hash_map_node_container_of(iter, struct k_str_map_node, hash_map_node);

            map->fn_free(map_node);
        }
    }

    map->fn_free(hash_map->buckets);
    map->fn_free(map);
}

struct k_str_map *k_str_map_construct(struct k_str_map *map, const struct k_str_map_options *options) {
    assert(NULL != map);

    if (NULL == options) {
        options = &default_options;
    }

    size_t buckets_num = 17;
    struct k_hash_list *buckets = options->fn_malloc(sizeof(struct k_hash_list) * buckets_num);
    if (NULL == buckets) {
        options->fn_free(map);
        return NULL;
    }

    map->fn_malloc = options->fn_malloc;
    map->fn_free   = options->fn_free;
    map->size      = 0;
    map->rehash_threshold = 29;
    k_str_hash_map_init(&map->hash_map, buckets, buckets_num);

    return map;
}

void k_str_map_destruct(struct k_str_map *map) {

    if (NULL == map)
        return;

    struct k_str_map_node *map_node;
    struct k_str_hash_map *hash_map = &map->hash_map;
    struct k_hash_list *buckets;
    for (k_str_hash_map_for_each_bucket(hash_map, buckets)) {
        struct k_hash_list_node *iter, *next;
        for (k_hash_list_for_each_s(buckets, iter, next)) {
            map_node = k_str_hash_map_node_container_of(iter, struct k_str_map_node, hash_map_node);

            map->fn_free(map_node);
        }
    }

    map->fn_free(hash_map->buckets);
}

static void rehash(struct k_str_map *map) {

    static const size_t primes[] = {
        53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593,
        49157, 98317, 196613, 393241, 786433, 1572869, 3145739,
        6291469, 12582917, 25165843, 50331653, 100663319,
        201326611, 402653189, 805306457, 1610612741
    };
    const size_t end = sizeof(primes) / sizeof(primes[0]);

    size_t old_buckets_num = map->hash_map.buckets_num;

    size_t left  = 0;
    size_t right = end;
    while (left < right) {
        size_t mid = (left + right) / 2;
        if (primes[mid] <= old_buckets_num)
            left = mid + 1;
        else
            right = mid;
    }

    if (right == end) {
        map->rehash_threshold = SIZE_MAX;
        return;
    }

    size_t new_buckets_num = primes[right];

    struct k_hash_list *new_buckets = map->fn_malloc(sizeof(struct k_hash_list) * new_buckets_num);
    if (NULL == new_buckets)
        return;

    struct k_hash_list *old_buckets = k_str_hash_map_rehash(&map->hash_map, new_buckets, new_buckets_num);
    map->rehash_threshold = (new_buckets_num / 4) * 3;

    map->fn_free(old_buckets);
}

void *k_str_map_put(struct k_str_map *map, const char *key, size_t value_size) {
    assert(NULL != map);
    assert(NULL != key && '\0' != key[0]);
    assert(0 < value_size && value_size < SIZE_MAX - sizeof(struct k_str_map_node));

    struct k_str_map_node *map_node = map->fn_malloc(sizeof(struct k_str_map_node) + value_size);
    if (NULL == map_node)
        return NULL;

    struct k_str_hash_map *hash_map = &map->hash_map;
    struct k_str_hash_map_node *hash_map_node = k_str_hash_map_get(hash_map, key);
    if (NULL != hash_map_node) {
        struct k_str_map_node *old_map_node = container_of(hash_map_node, struct k_str_map_node, hash_map_node);

        k_str_hash_map_del(&old_map_node->hash_map_node);
        map->fn_free(old_map_node);

        k_str_hash_map_add_directly(hash_map, key, &map_node->hash_map_node);
    }
    else {
        k_str_hash_map_add_directly(hash_map, key, &map_node->hash_map_node);
        map->size += 1;

        if (map->rehash_threshold < map->size) {
            rehash(map);
        }
    }

    return get_val(map_node);
}

void *k_str_map_add(struct k_str_map *map, const char *key, size_t value_size) {
    assert(NULL != map);
    assert(NULL != key && '\0' != key[0]);
    assert(0 < value_size && value_size < SIZE_MAX - sizeof(struct k_str_map_node));

    struct k_str_map_node *map_node = map->fn_malloc(sizeof(struct k_str_map_node) + value_size);
    if (NULL == map_node)
        return NULL;

    struct k_str_hash_map *hash_map = &map->hash_map;
    struct k_str_hash_map_node *hash_map_node = k_str_hash_map_get(hash_map, key);
    if (NULL != hash_map_node)
        return NULL;

    k_str_hash_map_add_directly(hash_map, key, &map_node->hash_map_node);
    map->size += 1;

    if (map->rehash_threshold < map->size) {
        rehash(map);
    }

    return get_val(map_node);
}

void k_str_map_del(struct k_str_map *map, const char *key) {
    assert(NULL != map);
    assert(NULL != key && '\0' != key[0]);

    struct k_str_hash_map_node *hash_map_node = k_str_hash_map_get(&map->hash_map, key);
    if (NULL == hash_map_node)
        return;

    struct k_str_map_node *map_node = container_of(hash_map_node, struct k_str_map_node, hash_map_node);

    k_str_hash_map_del(&map_node->hash_map_node);
    map->fn_free(map_node);

    map->size -= 1;
}

void *k_str_map_get(struct k_str_map *map, const char *key) {
    assert(NULL != map);
    assert(NULL != key && '\0' != key[0]);

    struct k_str_hash_map_node *hash_map_node = k_str_hash_map_get(&map->hash_map, key);
    if (NULL == hash_map_node) {
        return NULL;
    }
    else {
        struct k_str_map_node *map_node = container_of(hash_map_node, struct k_str_map_node, hash_map_node);
        return get_val(map_node);
    }
}

void k_str_map_clear(struct k_str_map *map) {
    assert(NULL != map);

    struct k_str_map_node *map_node;
    struct k_str_hash_map *hash_map = &map->hash_map;
    struct k_hash_list *buckets;
    for (k_str_hash_map_for_each_bucket(hash_map, buckets)) {
        struct k_hash_list_node *iter, *next;
        for (k_hash_list_for_each_s(buckets, iter, next)) {
            map_node = k_str_hash_map_node_container_of(iter, struct k_str_map_node, hash_map_node);

            map->fn_free(map_node);
        }
    }

    map->size = 0;
}
