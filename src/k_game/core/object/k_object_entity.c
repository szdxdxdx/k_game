#include "./_internal.h"

#include "../game/k_game_context.h"
#include "../room/_shared.h"

/* region [object_create] */

struct k_object *k_object_create(size_t object_data_size) {

    struct k_room *room = k__game.current_room;

    struct k_object *object = k__object_pool_acquire(&room->object_pool);
    if (NULL == object)
        goto err;

    if (0 == object_data_size) {
        object->data = NULL;
    } else {
        object->data = k_malloc(object_data_size);
        if (NULL == object->data) {
            k__object_pool_release(object);
            goto err;
        }
    }

    k_list_init(&object->callback_list);
    k_list_init(&object->component_list);

    object->room = room;
    object->fn_destroy = NULL;

    return object;

err:
    return NULL;
}

void k_object_destroy(struct k_object *object) {

    if (NULL == object)
        return;

    if (NULL != object->fn_destroy)
        object->fn_destroy(object);

    k_object_del_all_components(object);
    k_object_del_all_callbacks(object);

    k_free(object->data);
    k__object_pool_release(object);
}

/* endregion */

/* region [object_get/set] */

void *k_object_get_data(struct k_object *object) {
    return object->data;
}

void k_object_set_destroy_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {
    object->fn_destroy = fn_callback;
}

/* endregion */
