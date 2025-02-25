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

    return k__room_object_pool_new(room, data_size);
}

void k_destroy_object(struct k_object *object) {

    if (NULL != object)
        k__room_object_pool_del(object);
}

void *k_object_get_data(struct k_object *object) {
    return object->data;
}
