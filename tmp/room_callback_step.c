#include "k_game_alloc.h"
#include "./room.h"

void k__room_init_step_callback_registry(struct k_room *room) {
    struct k_room_step_callback_registry *registry = &room->step_callbacks;

    k_list_init(&registry->callback_list);
    k_list_init(&registry->pending_list);
}

void k__room_cleanup_step_callback_registry(struct k_room *room) {
    struct k_room_step_callback_registry *registry = &room->step_callbacks;

    struct k_room_step_callback *callback;
    struct k_list *list;
    struct k_list_node *iter, *next;

    list = &registry->callback_list;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_room_step_callback, list_node);
        k_free(callback);
    }

    list = &registry->pending_list;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_room_step_callback, list_node);
        k_free(callback);
    }
}

struct k_room_callback *k__room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    struct k_room_step_callback_registry *registry = &room->step_callbacks;

    struct k_room_step_callback *callback = k_malloc(sizeof(struct k_room_step_callback));
    if (NULL == callback)
        return NULL;

    callback->base.is_deleted = 0;
    callback->data = data;
    callback->fn_callback = fn_callback;
    k_list_add_tail(&registry->pending_list, &callback->list_node);

    return &callback->base;
}

void k__room_exec_step_callbacks(struct k_room *room) {
    struct k_room_step_callback_registry *registry = &room->step_callbacks;

    struct k_room_step_callback *callback;
    struct k_list *callback_list = &registry->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_list, iter, next)) {
        callback = container_of(iter, struct k_room_step_callback, list_node);

        if (callback->base.is_deleted) {
            k_list_del(&callback->list_node);
            k_free(callback);
        } else {
            callback->fn_callback(callback->data);
        }
    }
}

void k__room_flush_step_callbacks(struct k_room *room) {
    struct k_room_step_callback_registry *registry = &room->step_callbacks;

    struct k_list *callback_list = &registry->callback_list;
    struct k_list *pending_list  = &registry->pending_list;

    struct k_room_step_callback *callback;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(pending_list, iter, next)) {
        callback = container_of(iter, struct k_room_step_callback, list_node);

        k_list_del(&callback->list_node);
        k_list_add_tail(callback_list, &callback->list_node);
    }
}
