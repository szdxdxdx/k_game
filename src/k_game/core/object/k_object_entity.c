#include "./_internal.h"

#include "../game/_public.h"
#include "../room/_public.h"

/* region [object_create] */

struct k_object *k_object_create(size_t object_data_size) {

    struct k_room *room = k__game.current_room;
    if (NULL == room)
        goto err;

    struct k_object *object = k__object_pool_acquire(&room->object_pool);
    if (NULL == object)
        goto err;

    object->room = room;

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

    return object;

err:
    return NULL;
}

void k_object_destroy(struct k_object *object) {

    if (NULL == object)
        return;

    k_object_del_all_components(object);
    k__object_free_all_callbacks(object);

    k_free(object->data);
    k__object_pool_release(object);
}

/* endregion */

/* region [object_get] */

void *k_object_get_data(struct k_object *object) {
    return object->data;
}

/* endregion */
