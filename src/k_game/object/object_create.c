#include "k_log.h"

#include "k_game_alloc.h"
#include "k_game/room_context.h"
#include "k_game/object_pool.h"
#include "k_game/object_entity.h"
#include "k_game/object_callback.h"
#include "k_game/object_component.h"

struct k_object *k_object_create(struct k_room *room, size_t object_data_size) {
    struct k_object_pool *pool = &room->object_pool;

    struct k_object *object = k__object_pool_acquire(pool);
    if (NULL == object)
        goto err;

    void *data = NULL;
    if (0 != object_data_size) {
        if (NULL == (data = k_malloc(object_data_size))) {
            k__object_pool_release(object);
            goto err;
        }
    }

    object->room = room;
    object->data = data;
    k__object_init_callback_list(object);
    k__object_init_component_list(object);

    return object;

err:
    k_log_error("Failed to create object");
    return NULL;
}

void k__object_destroy(struct k_object *object) {

    k__object_cleanup_component_list(object);
    k__object_cleanup_callback_list(object);

    k_free(object->data);
    k__object_pool_release(object);
}

void k_object_destroy(struct k_object *object) {

    if (NULL != object)
        k__object_destroy(object);
}
