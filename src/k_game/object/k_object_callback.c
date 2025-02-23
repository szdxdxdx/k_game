
#include "k_game/alloc.h"
#include "k_game/object.h"
#include "k_game/room.h"
#include "./k_object_entity.h"
#include "./k_object_callback.h"

void k__object_init_callbacks_list(struct k_object *object) {
    struct k_object_callbacks_list *callbacks_list = &object->callbacks_list;

    k_list_init(&callbacks_list->list);
}

void k__object_cleanup_callbacks_list(struct k_object *object) {
    struct k_object_callbacks_list *callbacks_list = &object->callbacks_list;

    /* TODO foreach callbacks_list : del callback */
}

static inline void object_callbacks_list_add(struct k_object_callbacks_list *list, struct k_object_callback *callback) {
    k_list_add_tail(&list->list, &callback->iter_node);
}

static inline void object_callbacks_list_del(struct k_object_callback *callback) {
    k_list_del(&callback->iter_node);
}

static void room_step_callback_wrapper(void *data) {
    struct k_object_callback *callback = data;

    callback->fn_step(callback->object);
}

struct k_object_callback *k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    struct k_object_callback *object_callback = k_malloc(sizeof(struct k_object_callback));
    if (NULL == object_callback)
        return NULL;

    object_callback->fn_step = fn_callback;
    object_callback->object  = object;

    struct k_room_callback *room_callback = k_room_add_step_callback(object->room, room_step_callback_wrapper, object_callback);
    if (NULL == room_callback) {
        k_free(object_callback);
        return NULL;
    }

    object_callbacks_list_add(&object->callbacks_list, object_callback);
    object_callback->room_callback = room_callback;

    return object_callback;
}

void k_object_del_callback(struct k_object_callback *callback) {
    k_room_del_callback(callback->room_callback);
    object_callbacks_list_del(callback);
    k_free(callback);
}
