#include "./k_object_pool.h"

void k__init_object_pool(struct k_object_pool *pool) {
    k_list_init(&pool->objects_list);
}

void k__deinit_object_pool(struct k_object_pool *pool) {

    /* TODO */
}

void k__object_pool_add(struct k_object_pool *pool, struct k_object_pool_node *node) {

    k_list_add_tail(&pool->objects_list, &node->iter_node);
}

void k__object_pool_del(struct k_object_pool_node *node) {

    k_list_del(&node->iter_node);
}
