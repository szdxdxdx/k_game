#include "k_game_alloc.h"
#include "k_game/room_context.h"
#include "k_game/object_entity.h"

void k__room_init_object_pool(struct k_room *room) {
    struct k_object_pool *pool = &room->object_pool;

    k_list_init(&pool->object_list);
}

void k__room_deinit_object_pool(struct k_room *room) {
    struct k_object_pool *pool = &room->object_pool;

    struct k_list_node *iter, *next;
    for (k_list_for_each_s(&pool->object_list, iter, next)) {
        struct k_object *object = container_of(iter, struct k_object, pool_node);

        k__destroy_object(object);
    }
}

void k__room_object_pool_add(struct k_room *room, struct k_object *object) {
    struct k_object_pool *pool = &room->object_pool;
    k_list_add_tail(&pool->object_list, &object->pool_node.iter_node);
}

void k__room_object_pool_del(struct k_object *object) {
    k_list_del(&object->pool_node.iter_node);
}
