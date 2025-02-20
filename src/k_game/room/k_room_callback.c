#include "k_game/alloc.h"

#include "./k_room_context.h"

/* region [callback_list] */

struct k_room_callback_list_item {

    struct k_list_node list_node;

    struct k_room_callback impl;

    void *data;

    void (*fn_callback)(void *data);
};

void k__room_callback_list_init(struct k_room_callback_list *list) {
    k_list_init(&list->list);
}

void k__room_callback_list_clean(struct k_room_callback_list *list) {

    struct k_list_node *iter_node, *next;
    for (k_list_for_each_s(&list->list, iter_node, next)) {

        struct k_room_callback_list_item *callback;
        callback = container_of(iter_node, struct k_room_callback_list_item, list_node);

        k_free(callback);
    }
}

void k__room_callback_list_exec_all(struct k_room_callback_list *list) {

    struct k_list_node *iter_node, *next;
    for (k_list_for_each_s(&list->list, iter_node, next)) {

        struct k_room_callback_list_item *callback;
        callback = container_of(iter_node, struct k_room_callback_list_item, list_node);

        callback->fn_callback(callback->data);
    }
}

static void k__room_callback_list_item_del_self(struct k_room_callback *self) {

    struct k_room_callback_list_item *callback;
    callback = container_of(self, struct k_room_callback_list_item, impl);

    k_list_del(&callback->list_node);
    k_free(callback);
}

static struct k_room_callback *k__room_callback_list_add(struct k_room_callback_list *list, void (*fn_callback)(void *data), void *data) {

    struct k_room_callback_list_item *callback = k_malloc(sizeof(struct k_room_callback_list_item));
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&list->list, &callback->list_node);
    callback->impl.fn_del_self = k__room_callback_list_item_del_self;
    callback->data = data;
    callback->fn_callback = fn_callback;

    return &callback->impl;
}

/* endregion */

void k_room_del_callback(struct k_room_callback *callback) {

    if (NULL != callback)
        callback->fn_del_self(callback);
}

struct k_room_callback *k_room_add_enter_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return k__room_callback_list_add(&room->enter_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_leave_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return k__room_callback_list_add(&room->leave_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_step_begin_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return k__room_callback_list_add(&room->step_begin_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return k__room_callback_list_add(&room->step_callbacks, fn_callback, data);
}

struct k_room_callback *k_room_add_step_end_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return k__room_callback_list_add(&room->step_end_callbacks, fn_callback, data);
}
