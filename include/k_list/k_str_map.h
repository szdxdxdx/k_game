#ifndef K_STR_MAP_H
#define K_STR_MAP_H

#include <stddef.h>

#include "k_hash_list.h"

struct k_str_map {

    struct k_hash_list *buckets;

    size_t buckets_num;
};

struct k_str_map_node {

    struct k_hash_list_node list_node;

    const char *key;

    size_t hash;
};

void k_str_map_init(struct k_str_map *map, struct k_hash_list *buckets, size_t buckets_num);

int k_str_map_add(struct k_str_map *map, const char *key, struct k_str_map_node *node);

struct k_str_map_node *k_str_map_get(struct k_str_map *map, const char *key);

void k_str_map_del(struct k_str_map_node *node);

struct k_hash_list *k_str_map_rehash(struct k_str_map *map, struct k_hash_list *new_buckets, size_t new_buckets_num);

#endif
