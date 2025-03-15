#include "k_game.h"

#include "../room/k_room.h"
#include "../game/k_game_context.h"

#include "./k_object.h"

/* region [object_create] */

struct k_object *k_object_create(size_t object_data_size) {

    struct k_room *room = k__game.current_room;
    if (NULL == room) {
        goto err;
    }

    struct k_object *object = k__object_pool_acquire(&room->object_pool);
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
    k_list_init(&object->callback_list);
    k_list_init(&object->component_list);

    return object;

err:
    return NULL;
}

void k_object_destroy(struct k_object *object) {

    if (NULL == object)
        return;

    k_object_del_all_components(object);
    k_object_del_all_callbacks(object);

    k_free(object->data);
    k__object_pool_release(object);
}

/* endregion */

/* region [object_get] */

void *k_object_get_data(struct k_object *object) {
    return object->data;
}

/* endregion */
