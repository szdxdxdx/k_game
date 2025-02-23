#include "k_log.h"

#include "k_game/alloc.h"
#include "k_game/room.h"
#include "../room/k_room_context.h"
#include "./k_object_entity.h"

struct k_object *k_create_object(size_t data_size) {

    struct k_room *room = k_get_current_room();
    if (NULL == room) {
        k_log_error("Currently not in any room, unable to create object");
        return NULL;
    }

    struct k_object *object = k_malloc(sizeof(struct k_object));
    if (NULL == object)
        return NULL;

    if (0 == data_size) {
        object->data = 0;
    }
    else {
        if (NULL == (object->data = k_malloc(data_size))) {
            k_free(object);
            return NULL;
        }
    }

    object->room = room;
    k__object_pool_add(&room->object_pool, &object->object_node);
    k__object_init_callbacks_list(object);

    return object;
}

void k_destroy_object(struct k_object *object) {

    /* TODO */
}

void *k_object_get_data(struct k_object *object) {
    return object->data;
}