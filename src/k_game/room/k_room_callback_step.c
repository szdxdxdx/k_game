#include "k_game/alloc.h"

#include "./k_room_context.h"

void k__room_init_step_callbacks_list(struct k_room_step_callbacks_list *list) {
    k_list_init(&list->callbacks_list);
    k_list_init(&list->pending_list);
    k_list_init(&list->recycle_list);
}

void k__room_cleanup_step_callbacks_list(struct k_room_step_callbacks_list *list) {

    struct k_room_step_callback *callback;
    struct k_list_node *list_node, *next_node;

    for (k_list_for_each_s(&list->pending_list, list_node, next_node)) {
        callback = container_of(list_node, struct k_room_step_callback, pending_list_node);
        k_free(callback);
    }

    for (k_list_for_each_s(&list->callbacks_list, list_node, next_node)) {
        callback = container_of(list_node, struct k_room_step_callback, callbacks_list_node);
        k_free(callback);
    }

    for (k_list_for_each_s(&list->recycle_list, list_node, next_node)) {
        callback = container_of(list_node, struct k_room_step_callback, recycle_list_node);
        k_free(callback);
    }
}

void k__room_exec_step_callbacks(struct k_room_step_callbacks_list *list) {

    struct k_room_step_callback *callback;

    struct k_list_node *list_node;
    for (k_list_for_each(&list->callbacks_list, list_node)) {
        callback = container_of(list_node, struct k_room_step_callback, callbacks_list_node);

        callback->fn_callback(callback->data);
    }
}

static void del_self_from_pending_list(struct k_room_callback *self) {
    struct k_room_step_callback *callback = container_of(self, struct k_room_step_callback, impl);

    k_list_del(&callback->pending_list_node);
    k_free(callback);
}

static void del_self_from_recycle_list(struct k_room_callback *self) {
    (void)self;

    /* do nothing */
}

static void del_self_from_callbacks_list(struct k_room_callback *self) {
    struct k_room_step_callback *callback = container_of(self, struct k_room_step_callback, impl);

    struct k_list *recycle_list = callback->recycle_list;
    k_list_add_tail(recycle_list, &callback->recycle_list_node);
    callback->impl.fn_del_self = del_self_from_recycle_list;
}

void k__room_flush_pending_step_callbacks(struct k_room_step_callbacks_list *list) {

    if (k_list_is_empty(&list->pending_list))
        return;

    struct k_room_step_callback *callback;

    struct k_list_node *list_node;
    for (k_list_for_each(&list->pending_list, list_node)) {
        callback = container_of(list_node, struct k_room_step_callback, pending_list_node);

        k_list_add_tail(&list->callbacks_list, &callback->callbacks_list_node);
        callback->recycle_list = &list->recycle_list;
        callback->impl.fn_del_self = del_self_from_callbacks_list;
    }

    k_list_init(&list->pending_list);
}

void k__room_cleanup_recycled_step_callbacks(struct k_room_step_callbacks_list *list) {

    if (k_list_is_empty(&list->recycle_list))
        return;

    struct k_room_step_callback *callback;

    struct k_list_node *list_node, *next_node;
    for (k_list_for_each_s(&list->recycle_list, list_node, next_node)) {
        callback = container_of(list_node, struct k_room_step_callback, recycle_list_node);

        k_free(callback);
    }

    k_list_init(&list->recycle_list);
}

static struct k_room_callback *add_step_callback(struct k_room_step_callbacks_list *list, void (*fn_callback)(void *data), void *data) {

    struct k_room_step_callback *callback = k_malloc(sizeof(struct k_room_step_callback));
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&list->pending_list, &callback->pending_list_node);
    callback->impl.fn_del_self = del_self_from_pending_list;
    callback->data = data;
    callback->fn_callback = fn_callback;

    return &callback->impl;
}

struct k_room_callback *k_room_add_step_begin_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return add_step_callback(&room->step_begin_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return add_step_callback(&room->step_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_step_end_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return add_step_callback(&room->step_end_callbacks, fn_callback, data);
}
