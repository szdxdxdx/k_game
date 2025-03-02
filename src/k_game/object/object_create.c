#include "k_log.h"

#include "k_game_alloc.h"
#include "k_game_room.h"
#include "k_game/object_entity.h"
#include "k_game/component_entity.h"

struct k_object *k_create_object(size_t data_size) {
    const char *err_msg = "";

    struct k_room *room = k_get_current_room();
    if (NULL == room) {
        err_msg = "currently not in any room";
        goto err;
    }

    struct k_object *object = k_malloc(sizeof(struct k_object));
    if (NULL == object)
        goto err;

    void *data = NULL;
    if (0 != data_size) {
        if (NULL == (data = k_malloc(data_size))) {
            k_free(object);
            goto err;
        }
    }

    object->data = data;
    object->room = room;
    k__object_init_callbacks_list(object);
    k__object_init_component_list(object);
    k__room_object_pool_add(room, object);

    return object;

err:
    k_log_error("Failed to create object: %s", err_msg);
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

    if (NULL != object)
        k__destroy_object(object);
}
