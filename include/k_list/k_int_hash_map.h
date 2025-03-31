#ifndef K_INT_HASH_MAP_H
#define K_INT_HASH_MAP_H

#include <stddef.h>

#include "k_hash_list.h"

struct k_int_hash_map {

    struct k_hash_list *buckets;

    size_t buckets_num;
};

struct k_int_hash_map_node {

    struct k_hash_list_node list_node;

    int key;
};

void k_int_hash_map_init(struct k_int_hash_map *map, struct k_hash_list *buckets, size_t buckets_num);

struct k_int_hash_map_node *k_int_hash_map_put(struct k_int_hash_map *map, int key, struct k_int_hash_map_node *node);

int k_int_hash_map_add_if_absent(struct k_int_hash_map *map, int key, struct k_int_hash_map_node *node);

void k_int_hash_map_add_directly(struct k_int_hash_map *map, int key, struct k_int_hash_map_node *node);

struct k_int_hash_map_node *k_int_hash_map_get(struct k_int_hash_map *map, int key);

void k_int_hash_map_del(struct k_int_hash_map_node *node);

struct k_hash_list *k_int_hash_map_rehash(struct k_int_hash_map *map, struct k_hash_list *new_buckets, size_t new_buckets_num);

#define k_int_hash_map_for_each_bucket(map, bucket) \
    bucket = (map)->buckets; \
    bucket < (map)->buckets + (map)->buckets_num; \
    bucket++

#define k_int_hash_map_node_container_of(hash_list_iter, type, member) \
    container_of(container_of((hash_list_iter), struct k_int_hash_map_node, list_node), type, member)

#endif
