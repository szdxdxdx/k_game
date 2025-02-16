#include "k_game/alloc.h"
#include "k_game/room.h"
#include "./k_room_callback.h"
#include "./k_room_context.h"

/* region [generic_callback_list] */

struct k_room_generic_callback {

    struct k_room_callback impl;

    struct k_list_node iter_node;

    void *data;

    void (*fn_callback)(void *data);
};

void k__room_generic_callback_list_init(struct k_room_generic_callback_list *list) {
    k_list_init(&list->list);
}

void k__room_generic_callback_list_clean(struct k_room_generic_callback_list *list) {

    struct k_list_node *iter_node, *next;
    for (k_list_for_each_s(&list->list, iter_node, next)) {

        struct k_room_generic_callback *callback;
        callback = container_of(iter_node, struct k_room_generic_callback, iter_node);

        k_free(callback);
    }
}

static void del_self(struct k_room_callback *self) {

    struct k_room_generic_callback *callback;
    callback = container_of(self, struct k_room_generic_callback, impl);

    k_list_del(&callback->iter_node);
    k_free(callback);
}

struct k_room_callback *k__room_generic_callback_list_add(struct k_room_generic_callback_list *list, void (*fn_callback)(void *data), void *data) {

    struct k_room_generic_callback *callback = k_malloc(sizeof(struct k_room_generic_callback));
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&list->list, &callback->iter_node);
    callback->impl.fn_del_self = del_self;
    callback->data = data;
    callback->fn_callback = fn_callback;

    return &callback->impl;
}

void k__room_generic_callback_list_exec_all(struct k_room_generic_callback_list *list) {

    struct k_list_node *iter_node, *next;
    for (k_list_for_each_s(&list->list, iter_node, next)) {

        struct k_room_generic_callback *callback;
        callback = container_of(iter_node, struct k_room_generic_callback, iter_node);

        callback->fn_callback(callback->data);
    }
}

/* endregion */

struct k_room_callback *k_room_add_enter_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return k__room_generic_callback_list_add(&room->enter_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_leave_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return k__room_generic_callback_list_add(&room->leave_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_step_begin_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return k__room_generic_callback_list_add(&room->step_begin_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return k__room_generic_callback_list_add(&room->step_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_step_end_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return k__room_generic_callback_list_add(&room->step_end_callbacks, fn_callback, data);
}

void k_room_del_callback(struct k_room_callback *callback) {

    if (NULL != callback)
        callback->fn_del_self(callback);
}
