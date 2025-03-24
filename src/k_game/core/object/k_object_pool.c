#include "./_internal.h"

#include "../room/_public.h"

int k__object_pool_init(struct k_object_pool *pool) {

    k_list_init(&pool->object_list);
    return 0;
}

void k__object_pool_cleanup(struct k_object_pool *pool) {

    struct k_object *object;
    struct k_list *list = &pool->object_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        object = container_of(iter, struct k_object, pool_node);

        k_destroy_object(object);
    }
}

struct k_object *k__object_pool_acquire(struct k_object_pool *pool) {

    struct k_object *object = k_malloc(sizeof(struct k_object)); /* TODO pool_alloc */
    if (NULL == object)
        return NULL;

    k_list_add_tail(&pool->object_list, &object->pool_node.iter_node);
    return object;
}

void k__object_pool_release(struct k_object *object) {
    k_list_del(&object->pool_node.iter_node);
    k_free(object); /* TODO pool_free */
}
