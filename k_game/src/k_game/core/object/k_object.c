#include "k_game/core/k_mem_alloc.h"
#include "k_game/core/k_object.h"

#include "./k_object.h"
#include "./k_object_pool.h"

#include "../room/k_room.h"
#include "../component/k_component.h"
#include "../callback/k_callback_base.h"

/* region [object_create] */

struct k_object *k_object_create(size_t data_size) {
    struct k_room *room = k__room_current;

    struct k_object *object = k__object_pool_acquire(&room->object_pool);
    if (NULL == object)
        return NULL;

    if (0 == data_size) {
        object->data = NULL;
    } else {
        object->data = k__mem_alloc(data_size);
        if (NULL == object->data) {
            k__object_pool_release(object);
            return NULL;
        }
    }

    k_list_init(&object->callback_list);
    k_list_init(&object->component_list);

    object->room = room;
    object->on_destroy = NULL;

    return object;
}

void k_object_destroy(struct k_object *object) {

    if (NULL == object)
        return;

    if (NULL != object->on_destroy) {
        object->on_destroy(object);
    }

    k__object_del_all_components(object);
    k__object_del_all_callbacks(object);

    k__mem_free(object->data);
    k__object_pool_release(object);
}

/* endregion */

/* region [object_get/set] */

void *k_object_get_data(struct k_object *object) {
    return object->data;
}

void k_object_set_on_destroy(struct k_object *object, void (*on_destroy)(struct k_object *object)) {
    object->on_destroy = on_destroy;
}

/* endregion */
