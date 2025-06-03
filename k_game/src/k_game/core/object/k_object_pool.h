#ifndef K__OBJECT_POOL_H
#define K__OBJECT_POOL_H

#include "k_list.h"
#include "k_intrusive_map.h"

struct k_object;

struct k_object_pool {

    struct k_list object_list;

    struct k_int_intrusive_map id_map;
};

struct k_object_pool_node {

    struct k_list_node object_list_node;

    struct k_int_intrusive_map_node id_map_node;
};

int k__object_pool_init(struct k_object_pool *pool);

void k__object_pool_cleanup(struct k_object_pool *pool);

struct k_object *k__object_pool_acquire(struct k_object_pool *pool);

void k__object_pool_release(struct k_object *object);

#endif
