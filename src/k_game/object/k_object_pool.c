#include "k_game/alloc.h"
#include "k_game/object.h"
#include "../room/k_room_context.h"
#include "./k_object_entity.h"

void k__room_init_object_pool(struct k_room *room) {
    struct k_object_pool *pool = &room->object_pool;

    k_list_init(&pool->objects_list);
}

void k__room_cleanup_object_pool(struct k_room *room) {
    struct k_object_pool *pool = &room->object_pool;

    struct k_list_node *iter_node, *next;
    for (k_list_for_each_s(&pool->objects_list, iter_node, next)) {
        struct k_object *object = container_of(iter_node, struct k_object, object_node);

        k_destroy_object(object);
    }
}

struct k_object *k__room_object_pool_new(struct k_room *room, size_t object_data_size) {
    struct k_object_pool *pool = &room->object_pool;

    struct k_object *object = k_malloc(sizeof(struct k_object)); /* <- TODO 池化 */
    if (NULL == object)
        return NULL;

    if (0 == object_data_size) {
        object->data = 0;
    }
    else {
        if (NULL == (object->data = k_malloc(object_data_size))) {
            k_free(object);
            return NULL;
        }
    }

    object->room = room;
    k_list_add_tail(&pool->objects_list, &object->object_node.iter_node);
    k__object_init_callbacks_list(object);

    return object;
}

void k__room_object_pool_del(struct k_object *object) {

    /* 将 object 从 object pool 中摘除 */
    k_list_del(&object->object_node.iter_node);

    /* 删除 object 携带的 callback */
    k__object_cleanup_callbacks_list(object);

    /* 释放 object 所占的内存 */
    k_free(object->data);
    k_free(object);
}
