#include "k_game_alloc.h"
#include "k_game/room_context.h"

/* region [init] */

void k__room_init_step_callback_registry(struct k_room *room) {
    struct k_room_step_callback_registry *registry = &room->step_callbacks;

    k_list_init(&registry->step_begin_callback_list);
    k_list_init(&registry->step_callback_list);
    k_list_init(&registry->step_end_callback_list);

    k_list_init(&registry->step_begin_pending_list);
    k_list_init(&registry->step_pending_list);
    k_list_init(&registry->step_end_pending_list);
}

/* endregion */

/* region [cleanup] */

static inline void free_callbacks(struct k_list *list) {

    struct k_room_step_callback *callback;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_room_step_callback, list_node);
        k_free(callback);
    }
}

void k__room_cleanup_step_callback_registry(struct k_room *room) {
    struct k_room_step_callback_registry *registry = &room->step_callbacks;

    free_callbacks(&registry->step_begin_callback_list);
    free_callbacks(&registry->step_callback_list);
    free_callbacks(&registry->step_end_callback_list);
    free_callbacks(&registry->step_begin_pending_list);
    free_callbacks(&registry->step_pending_list);
    free_callbacks(&registry->step_end_pending_list);
}

/* endregion */

/* region [add_callback] */

static inline struct k_room_callback *add_callback(struct k_list *pending_list, void (*fn_callback)(void *data), void *data) {

    struct k_room_step_callback *callback = k_malloc(sizeof(struct k_room_step_callback));
    if (NULL == callback)
        return NULL;

    callback->base.is_deleted = 0;
    callback->data = data;
    callback->fn_callback = fn_callback;
    k_list_add_tail(pending_list, &callback->list_node);

    return &callback->base;
}

struct k_room_callback *k__room_add_step_begin_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return add_callback(&room->step_callbacks.step_begin_pending_list, fn_callback, data);
}

struct k_room_callback *k__room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return add_callback(&room->step_callbacks.step_pending_list, fn_callback, data);
}

struct k_room_callback *k__room_add_step_end_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return add_callback(&room->step_callbacks.step_end_pending_list, fn_callback, data);
}

/* endregion */

/* region [exec_callbacks] */

static inline void exec_callbacks(struct k_list *callback_list) {

    struct k_room_step_callback *callback;
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

void k__room_exec_step_begin_callbacks(struct k_room *room) {
    exec_callbacks(&room->step_callbacks.step_begin_callback_list);
}

void k__room_exec_step_callbacks(struct k_room *room) {
    exec_callbacks(&room->step_callbacks.step_callback_list);
}

void k__room_exec_step_end_callbacks(struct k_room *room) {
    exec_callbacks(&room->step_callbacks.step_end_callback_list);
}

/* endregion */

/* region [flush_pending] */

static inline void flush_pending(struct k_list *pending_list, struct k_list *callback_list) {

    struct k_room_step_callback *callback;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(pending_list, iter, next)) {
        callback = container_of(iter, struct k_room_step_callback, list_node);

        if (callback->base.is_deleted) {
            k_list_del(&callback->list_node);
            k_free(callback);
        } else {
            k_list_del(&callback->list_node);
            k_list_add_tail(callback_list, &callback->list_node);
        }
    }
}

void k__room_flush_step_begin_callbacks(struct k_room *room) {
    struct k_list *callback_list = &room->step_callbacks.step_begin_callback_list;
    struct k_list *pending_list  = &room->step_callbacks.step_begin_pending_list;
    flush_pending(pending_list, callback_list);
}

void k__room_flush_step_callbacks(struct k_room *room) {
    struct k_list *callback_list = &room->step_callbacks.step_callback_list;
    struct k_list *pending_list  = &room->step_callbacks.step_pending_list;
    flush_pending(pending_list, callback_list);
}

void k__room_flush_step_end_callbacks(struct k_room *room) {
    struct k_list *callback_list = &room->step_callbacks.step_end_callback_list;
    struct k_list *pending_list  = &room->step_callbacks.step_end_pending_list;
    flush_pending(pending_list, callback_list);
}

/* endregion */
