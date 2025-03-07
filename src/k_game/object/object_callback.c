#include "../room/room.h"
#include "./object.h"

void k__object_init_callback_list(struct k_object *object) {

    k_list_init(&object->callback_list);
}

void k__object_cleanup_callback_list(struct k_object *object) {

    /* TODO */
}

struct k_object_callback *k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    struct k_step_callback_manager *manager = &object->room->step_callback_manager;
    struct k_object_callback *callback = k__add_object_step_callback(manager, fn_callback, object);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&object->callback_list, &callback->list_node);
    return callback;
}
