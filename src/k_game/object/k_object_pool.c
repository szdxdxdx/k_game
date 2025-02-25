#include "k_game/object.h"
#include "./k_object_entity.h"

void k__init_object_pool(struct k_object_pool *pool) {
    k_list_init(&pool->objects_list);
}

void k__deinit_object_pool(struct k_object_pool *pool) {

    struct k_object *object = NULL;
    struct k_list_node *iter_node, *next;
    for (k_list_for_each_s(&pool->objects_list, iter_node, next)) {
        object = container_of(iter_node, struct k_object, object_node);

        k_destroy_object(object);
    }
}

void k__object_pool_add(struct k_object_pool *pool, struct k_object_pool_node *node) {

    k_list_add_tail(&pool->objects_list, &node->iter_node);
}

void k__object_pool_del(struct k_object_pool_node *node) {

    k_list_del(&node->iter_node);
}
