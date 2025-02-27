#include "k_game/alloc.h"
#include "./k_room_context.h"

/* region [callback_list] */

void k__room_callback_list_init(struct k_room_callback_list *list) {
    k_list_init(&list->list);
}

void k__room_callback_list_clean(struct k_room_callback_list *list) {

    struct k_room_callback_list_item *callback;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(&list->list, iter, next)) {
        callback = container_of(iter, struct k_room_callback_list_item, list_node);

        k_free(callback);
    }
}

void k__room_callback_list_exec_all(struct k_room_callback_list *list) {

    struct k_room_callback_list_item *callback;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(&list->list, iter, next)) {
        callback = container_of(iter, struct k_room_callback_list_item, list_node);

        callback->fn_callback(callback->data);
    }
}

static void room_callback_list_item_del_self(struct k_room_callback *self) {

    struct k_room_callback_list_item *callback;
    callback = container_of(self, struct k_room_callback_list_item, impl);

    k_list_del(&callback->list_node);
    k_free(callback);
}

static struct k_room_callback *room_callback_list_add(struct k_room_callback_list *list, void (*fn_callback)(void *data), void *data) {

    struct k_room_callback_list_item *callback = k_malloc(sizeof(struct k_room_callback_list_item));
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&list->list, &callback->list_node);
    callback->impl.fn_del_self = room_callback_list_item_del_self;
    callback->data = data;
    callback->fn_callback = fn_callback;

    return &callback->impl;
}

/* endregion */

/* region [use callback_list] */

struct k_room_callback *k_room_add_enter_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    /* TODO assert(NULL != room && NULL != fn_callback) */
    return room_callback_list_add(&room->enter_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_leave_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return room_callback_list_add(&room->leave_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_step_begin_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return room_callback_list_add(&room->step_begin_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return room_callback_list_add(&room->step_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_step_end_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return room_callback_list_add(&room->step_end_callbacks, fn_callback, data);
}

/* endregion */
