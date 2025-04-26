#ifndef K_STR_INTRUSIVE_MAP_H
#define K_STR_INTRUSIVE_MAP_H

#include <stddef.h>

#include "k_hash_list.h"

struct k_str_intrusive_map {

    struct k_hash_list *buckets;

    size_t buckets_num;
};

struct k_str_intrusive_map_node {

    struct k_hash_list_node node_link;

    const char *key;

    size_t hash;
};

void k_str_intrusive_map_init(struct k_str_intrusive_map *map, struct k_hash_list *buckets, size_t buckets_num);

int k_str_intrusive_map_add_if_absent(struct k_str_intrusive_map *map, const char *key, struct k_str_intrusive_map_node *node);

void k_str_intrusive_map_add_directly(struct k_str_intrusive_map *map, const char *key, struct k_str_intrusive_map_node *node);

struct k_str_intrusive_map_node *k_str_intrusive_map_get(struct k_str_intrusive_map *map, const char *key);

void k_str_intrusive_map_del(struct k_str_intrusive_map_node *node);

struct k_hash_list *k_str_intrusive_map_rehash(struct k_str_intrusive_map *map, struct k_hash_list *new_buckets, size_t new_buckets_num);

#define k_str_intrusive_map_for_each_bucket(map, bucket) \
    bucket = (map)->buckets; \
    bucket < (map)->buckets + (map)->buckets_num; \
    bucket++

#define k_str_intrusive_map_node_container_of(hash_list_iter, type, member) \
    container_of(container_of((hash_list_iter), struct k_str_intrusive_map_node, node_link), type, member)

#endif
