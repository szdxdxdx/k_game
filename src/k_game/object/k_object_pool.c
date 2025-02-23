#include "./k_object_pool.h"

void k__init_object_pool(struct k_object_pool *object_pool) {
    k_list_init(&object_pool->objects_list);
}

void k__deinit_object_pool(struct k_object_pool *object_pool) {

    /* TODO */
}
