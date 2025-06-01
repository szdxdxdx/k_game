#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "k_str_map.h"

struct k_str_map_node {
    struct k_hash_list_node node_link;

    char *key;
    size_t key_hash;

    void *val;
};

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

static const struct k_str_map_options default_options = {
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

    k_hash_list_init_all(buckets, buckets_num);

    map->fn_malloc   = options->fn_malloc;
    map->fn_free     = options->fn_free;
    map->size        = 0;
    map->buckets     = buckets;
    map->buckets_num = buckets_num;
    map->rehash_threshold = 29;

    return map;
}

void k_str_map_destroy(struct k_str_map *map) {

    if (NULL == map)
        return;

    void (*fn_free)(void *p) = map->fn_free;

    struct k_hash_list *bucket = map->buckets;
    struct k_hash_list *bucket_end = &bucket[map->buckets_num];
    for (;bucket < bucket_end; bucket++) {

        struct k_str_map_node *map_node;
        struct k_hash_list_node *iter, *next;
        for (k_hash_list_for_each_s(bucket, iter, next)) {
            map_node = container_of(iter, struct k_str_map_node, node_link);

            fn_free(map_node);
        }
    }

    fn_free(map->buckets);
    fn_free(map);
}

struct k_str_map *k_str_map_construct(struct k_str_map *map, const struct k_str_map_options *options) {
    assert(NULL != map);

    if (NULL == options) {
        options = &default_options;
    }

    size_t buckets_num = 17;
    struct k_hash_list *buckets = options->fn_malloc(sizeof(struct k_hash_list) * buckets_num);
    if (NULL == buckets)
        return NULL;

    k_hash_list_init_all(buckets, buckets_num);

    map->fn_malloc   = options->fn_malloc;
    map->fn_free     = options->fn_free;
    map->size        = 0;
    map->buckets     = buckets;
    map->buckets_num = buckets_num;
    map->rehash_threshold = 29;

    return map;
}

void k_str_map_destruct(struct k_str_map *map) {

    if (NULL == map)
        return;

    void (*fn_free)(void *p) = map->fn_free;

    struct k_hash_list *bucket = map->buckets;
    struct k_hash_list *bucket_end = &bucket[map->buckets_num];
    for (; bucket < bucket_end; bucket++) {

        struct k_str_map_node *map_node;
        struct k_hash_list_node *iter, *next;
        for (k_hash_list_for_each_s(bucket, iter, next)) {
            map_node = container_of(iter, struct k_str_map_node, node_link);

            fn_free(map_node);
        }
    }

    fn_free(map->buckets);
}

static size_t str_hash(const char *key, size_t *get_len) {

    size_t hash = 5381;

    const char *p = key;
    for (; '\0' != *p; p++) {
        hash = hash * 33 + *p;
    }

    if (NULL != get_len) {
        *get_len = p - key;
    }

    return hash;
}

static struct k_hash_list *select_bucket(struct k_hash_list *buckets, size_t buckets_num, size_t key_hash) {
    return &(buckets[key_hash % buckets_num]);
}

static void rehash(struct k_str_map *map) {

    static const size_t primes[] = {
        53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
        196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
        50331653, 100663319, 201326611, 402653189, 805306457, 1610612741
    };
    const size_t primes_end = sizeof(primes) / sizeof(primes[0]);

    size_t left  = 0;
    size_t right = primes_end;
    while (left < right) {
        size_t mid = (left + right) / 2;
        if (primes[mid] <= map->buckets_num)
            left = mid + 1;
        else
            right = mid;
    }

    if (right == primes_end) {
        map->rehash_threshold = SIZE_MAX;
        return;
    }

    size_t new_buckets_num = primes[right];

    struct k_hash_list *new_buckets = map->fn_malloc(sizeof(struct k_hash_list) * new_buckets_num);
    if (NULL == new_buckets)
        return;

    k_hash_list_init_all(new_buckets, new_buckets_num);

    struct k_hash_list *old_buckets = map->buckets;
    size_t old_buckets_num = map->buckets_num;

    struct k_hash_list *old_list = old_buckets;
    for (; old_list < old_buckets + old_buckets_num; old_list++) {

        struct k_hash_list_node *iter, *next;
        for (k_hash_list_for_each_s(old_list, iter, next)) {
            struct k_str_map_node *node = container_of(iter, struct k_str_map_node, node_link);

            struct k_hash_list *new_list = select_bucket(new_buckets, new_buckets_num, node->key_hash);
            k_hash_list_add(new_list, &node->node_link);
        }
    }

    map->rehash_threshold = (new_buckets_num / 4) * 3;
    map->buckets     = new_buckets;
    map->buckets_num = new_buckets_num;

    map->fn_free(old_buckets);
}

static struct k_str_map_node *find(struct k_hash_list *bucket, const char *key, size_t hash) {

    struct k_hash_list_node *iter;
    for (k_hash_list_for_each(bucket, iter)) {
        struct k_str_map_node *node = container_of(iter, struct k_str_map_node, node_link);

        if (node->key_hash == hash) {
            if (strcmp(node->key, key) == 0)
                return node;
        }
    }

    return NULL;
}

void *k_str_map_put(struct k_str_map *map, const char *key, size_t val_size) {
    assert(NULL != map);

    if (NULL == key)
        return NULL;
    if (0 == val_size || SIZE_MAX - sizeof(struct k_str_map_node) <= val_size)
        return NULL;

    size_t get_key_len;
    size_t key_hash = str_hash(key, &get_key_len);
    struct k_hash_list *bucket = select_bucket(map->buckets, map->buckets_num, key_hash);

    size_t key_size = get_key_len + 1;

    struct k_str_map_node *new_node = map->fn_malloc(sizeof(struct k_str_map_node) + key_size + val_size);
    if (NULL == new_node)
        return NULL;

    new_node->key_hash = key_hash;
    new_node->key = ptr_offset(new_node, sizeof(struct k_str_map_node));
    strcpy(new_node->key, key);

    new_node->val = ptr_offset(new_node, sizeof(struct k_str_map_node) + key_size);

    struct k_str_map_node *old_node = find(bucket, key, key_hash);
    if (NULL != old_node) {

        k_hash_list_remove(&old_node->node_link);
        map->fn_free(old_node);

        k_hash_list_add(bucket, &new_node->node_link);
    }
    else {
        k_hash_list_add(bucket, &new_node->node_link);
        map->size += 1;

        if (map->rehash_threshold < map->size) {
            rehash(map);
        }
    }

    return new_node->val;
}

void *k_str_map_put_ref(struct k_str_map *map, const char *key, size_t val_size) {
    assert(NULL != map);

    if (NULL == key)
        return NULL;
    if (0 == val_size || SIZE_MAX - sizeof(struct k_str_map_node) <= val_size)
        return NULL;

    size_t key_hash = str_hash(key, NULL);
    struct k_hash_list *bucket = select_bucket(map->buckets, map->buckets_num, key_hash);

    struct k_str_map_node *new_node = map->fn_malloc(sizeof(struct k_str_map_node) + val_size);
    if (NULL == new_node)
        return NULL;

    new_node->key_hash = key_hash;
    new_node->key = (char *)key;

    new_node->val = ptr_offset(new_node, sizeof(struct k_str_map_node));

    struct k_str_map_node *old_node = find(bucket, key, key_hash);
    if (NULL != old_node) {

        k_hash_list_remove(&old_node->node_link);
        map->fn_free(old_node);

        k_hash_list_add(bucket, &new_node->node_link);
    }
    else {
        k_hash_list_add(bucket, &new_node->node_link);
        map->size += 1;

        if (map->rehash_threshold < map->size) {
            rehash(map);
        }
    }

    return new_node->val;
}

void *k_str_map_add(struct k_str_map *map, const char *key, size_t val_size) {
    assert(NULL != map);

    if (NULL == key)
        return NULL;
    if (0 == val_size || SIZE_MAX - sizeof(struct k_str_map_node) <= val_size)
        return NULL;

    size_t get_key_len;
    size_t key_hash = str_hash(key, &get_key_len);
    struct k_hash_list *bucket = select_bucket(map->buckets, map->buckets_num, key_hash);

    struct k_str_map_node *old_node = find(bucket, key, key_hash);
    if (NULL != old_node)
        return NULL;

    size_t key_size = get_key_len + 1;

    struct k_str_map_node *new_node = map->fn_malloc(sizeof(struct k_str_map_node) + key_size + val_size);
    if (NULL == new_node)
        return NULL;

    new_node->key_hash = key_hash;
    new_node->key = ptr_offset(new_node, sizeof(struct k_str_map_node));
    strcpy(new_node->key, key);

    new_node->val = ptr_offset(new_node, sizeof(struct k_str_map_node) + key_size);

    k_hash_list_add(bucket, &new_node->node_link);
    map->size += 1;

    if (map->rehash_threshold < map->size) {
        rehash(map);
    }

    return new_node->val;
}

void *k_str_map_add_ref(struct k_str_map *map, const char *key, size_t val_size) {
    assert(NULL != map);

    if (NULL == key)
        return NULL;
    if (0 == val_size || SIZE_MAX - sizeof(struct k_str_map_node) <= val_size)
        return NULL;

    size_t key_hash = str_hash(key, NULL);
    struct k_hash_list *bucket = select_bucket(map->buckets, map->buckets_num, key_hash);

    struct k_str_map_node *old_node = find(bucket, key, key_hash);
    if (NULL != old_node)
        return NULL;

    struct k_str_map_node *new_node = map->fn_malloc(sizeof(struct k_str_map_node) + val_size);
    if (NULL == new_node)
        return NULL;

    new_node->key_hash = key_hash;
    new_node->key = (char *)key;

    new_node->val = ptr_offset(new_node, sizeof(struct k_str_map_node));

    k_hash_list_add(bucket, &new_node->node_link);
    map->size += 1;

    if (map->rehash_threshold < map->size) {
        rehash(map);
    }

    return new_node->val;
}

void k_str_map_remove(struct k_str_map *map, const char *key) {
    assert(NULL != map);

    if (NULL == key)
        return;

    size_t key_hash = str_hash(key, NULL);
    struct k_hash_list *bucket = select_bucket(map->buckets, map->buckets_num, key_hash);

    struct k_str_map_node *node = find(bucket, key, key_hash);
    if (NULL == node)
        return;

    k_hash_list_remove(&node->node_link);
    map->size -= 1;

    map->fn_free(node);
}

void *k_str_map_get(struct k_str_map *map, const char *key) {
    assert(NULL != map);

    if (NULL == key)
        return NULL;

    size_t hash = str_hash(key, NULL);
    struct k_hash_list *bucket = select_bucket(map->buckets, map->buckets_num, hash);

    struct k_str_map_node *node = find(bucket, key, hash);
    if (NULL == node)
        return NULL;

    return node->val;
}

void k_str_map_clear(struct k_str_map *map) {
    assert(NULL != map);

    void (*fn_free)(void *p) = map->fn_free;

    struct k_hash_list *bucket = map->buckets;
    struct k_hash_list *bucket_end = &bucket[map->buckets_num];
    for (; bucket < bucket_end; bucket++) {

        struct k_hash_list_node *iter, *next;
        for (k_hash_list_for_each_s(bucket, iter, next)) {
            struct k_str_map_node *node = container_of(iter, struct k_str_map_node, node_link);

            fn_free(node);
        }
    }

    k_hash_list_init_all(map->buckets, map->buckets_num);

    map->size = 0;
}

void k_str_map_clear_with_callback(struct k_str_map *map, void (*fn_callback)(const char *key, void *val)) {
    assert(NULL != map);

    if (NULL == fn_callback) {
        k_str_map_clear(map);
        return;
    }

    void (*fn_free)(void *p) = map->fn_free;

    struct k_hash_list *bucket = map->buckets;
    struct k_hash_list *bucket_end = &bucket[map->buckets_num];
    for (; bucket < bucket_end; bucket++) {

        struct k_hash_list_node *iter, *next;
        for (k_hash_list_for_each_s(bucket, iter, next)) {
            struct k_str_map_node *node = container_of(iter, struct k_str_map_node, node_link);

            fn_callback(node->key, node->val);
            fn_free(node);
        }
    }

    k_hash_list_init_all(map->buckets, map->buckets_num);

    map->size = 0;
}

void k_str_map_iter_init(struct k_str_map_iter *iter, struct k_str_map *map) {
    assert(NULL != iter);
    assert(NULL != map);

    struct k_str_map_node *node = NULL;

    struct k_hash_list *bucket = map->buckets;
    struct k_hash_list *bucket_end = &bucket[map->buckets_num];
    for (; bucket < bucket_end; bucket++) {

        if ( ! k_hash_list_is_empty(bucket)) {
            node = container_of(bucket->first, struct k_str_map_node, node_link);
            break;
        }
    }

    iter->map    = map;
    iter->bucket = bucket;
    iter->node   = node;
}

int k_str_map_iter_get(struct k_str_map_iter *iter, const char **get_key, void *get_val) {
    assert(NULL != iter);

    struct k_str_map_node *node = iter->node;
    if (NULL == node)
        return 0;

    if (NULL != get_key) { *get_key = node->key; }
    if (NULL != get_val) { *(void **)get_val = node->val; }

    return 1;
}

void k_str_map_iter_next(struct k_str_map_iter *iter) {
    assert(NULL != iter);

    struct k_str_map_node *map_node = iter->node;
    struct k_hash_list_node *list_node = &map_node->node_link;

    if (list_node->next != NULL) {
        iter->node = container_of(list_node->next, struct k_str_map_node, node_link);
        return;
    }

    struct k_str_map_node *next_map_node = NULL;

    struct k_str_map *map = iter->map;
    struct k_hash_list *buckets = map->buckets;
    struct k_hash_list *bucket_end = &buckets[map->buckets_num];
    struct k_hash_list *iter_bucket = iter->bucket;
    if (iter_bucket < bucket_end) {
        iter_bucket++;
        for (; iter_bucket < bucket_end; iter_bucket++) {
            if ( ! k_hash_list_is_empty(iter_bucket)) {
                next_map_node = container_of(iter_bucket->first, struct k_str_map_node, node_link);
                break;
            }
        }
    }

    iter->bucket = iter_bucket;
    iter->node   = next_map_node;
}
