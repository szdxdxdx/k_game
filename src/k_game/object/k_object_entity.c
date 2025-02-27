#include "k_log.h"

#include "k_game/alloc.h"
#include "k_game/room.h"
#include "../room/k_room_context.h"
#include "./k_object_entity.h"

/* region [create] */

struct k_object *k_create_object(size_t data_size) {

    struct k_room *room = k_get_current_room();
    if (NULL == room) {
        k_log_error("Currently not in any room, unable to create object");
        goto not_in_room;
    }

    struct k_object *object = k_malloc(sizeof(struct k_object));
    if (NULL == object)
        goto malloc_object_failed;

    void *data = NULL;
    if (0 != data_size) {
        if (NULL == (data = k_malloc(data_size)))
            goto malloc_data_failed;
    }

    object->data = data;
    object->room = room;
    k__object_init_callbacks_list(object);
    k__object_init_component_list(object);
    k__room_object_pool_add(room, object);

    return object;

malloc_data_failed:
    k_free(object);
malloc_object_failed:
not_in_room:
    return NULL;
}

void k__destroy_object(struct k_object *object) {

    k__object_cleanup_component_list(object);
    k__object_cleanup_callbacks_list(object);
    k__room_object_pool_del(object);

    k_free(object->data);
    k_free(object);
}

void k_destroy_object(struct k_object *object) {

    if (NULL == object)
        k__destroy_object(object);
}

/* endregion */

void *k_object_get_data(struct k_object *object) {
    return object->data;
}
