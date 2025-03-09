#ifndef K_INT_MAP_H
#define K_INT_MAP_H

#include <stddef.h>

#include "k_hash_list.h"

struct k_int_map {

    struct k_hash_list *buckets;

    size_t buckets_num;
};

struct k_int_map_node {

    struct k_hash_list_node list_node;

    int key;
};

void k_int_map_init(struct k_int_map *map, struct k_hash_list *buckets, size_t buckets_num);

struct k_int_map_node *k_int_map_put(struct k_int_map *map, int key, struct k_int_map_node *node);

int k_int_map_add_if_absent(struct k_int_map *map, int key, struct k_int_map_node *node);

void k_int_map_add_directly(struct k_int_map *map, int key, struct k_int_map_node *node);

struct k_int_map_node *k_int_map_get(struct k_int_map *map, int key);

void k_int_map_del(struct k_int_map_node *node);

struct k_hash_list *k_int_map_rehash(struct k_int_map *map, struct k_hash_list *new_buckets, size_t new_buckets_num);

#endif
