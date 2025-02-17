#include "k_game/alloc.h"
#include "../game/k_game_context.h"
#include "./k_room_callback_alarm.h"
#include "./k_room_context.h"

struct k_room_alarm_callback {

    struct k_room_callback impl;

    struct k_list_node list_node;

    void *data;

    void (*fn_callback)(void *data, int timeout_diff);

    uint64_t timeout;
};

void k__room_init_alarm_callbacks_storage(struct k_room *room) {
    struct k_room_alarm_callbacks_storage *storage = &room->alarm_callbacks;

    k_list_init(&storage->list);
}

void k__room_del_all_alarm_callbacks(struct k_room *room) {
    struct k_room_alarm_callbacks_storage *storage = &room->alarm_callbacks;

    struct k_list_node *node, *next;
    for (k_list_for_each_s(&storage->list, node, next)) {
        struct k_room_alarm_callback *callback = container_of(node, struct k_room_alarm_callback, list_node);

        k_free(callback);
    }
}

void k__room_exec_alarm_callbacks(struct k_room *room) {
    struct k_room_alarm_callbacks_storage *storage = &room->alarm_callbacks;

    const uint64_t current_ms = k_game.current_ms;

    struct k_room_alarm_callback *callback;
    struct k_list_node *iter_node, *next;
    for (k_list_for_each_s(&storage->list, iter_node, next)) {
        callback = container_of(iter_node, struct k_room_alarm_callback, list_node);

        if (callback->timeout <= current_ms) {

            int timeout_diff = (int)(current_ms - callback->timeout);
            callback->fn_callback(callback->data, timeout_diff);

            k_list_del(&callback->list_node);
            k_free(callback);
        }
    }
}

static void alarm_callback_del_self(struct k_room_callback *self) {
    struct k_room_alarm_callback *callback = container_of(self, struct k_room_alarm_callback, impl);

    k_list_del(&callback->list_node);
    k_free(callback);
}

struct k_room_callback *k_room_add_alarm_callback(struct k_room *room, void (*fn_callback)(void *data, int timeout_diff), void *data, int delay) {
    struct k_room_alarm_callbacks_storage *storage = &room->alarm_callbacks;

    if (delay < 0)
        delay = 0;

    struct k_room_alarm_callback *callback = k_malloc(sizeof(struct k_room_alarm_callback));
    if (NULL == callback)
        return NULL;

    uint64_t timeout = k_game.current_ms + delay;

    struct k_list_node *iter_node;
    for (k_list_for_each(&storage->list, iter_node)) {
        struct k_room_alarm_callback *callback_in_list = container_of(iter_node, struct k_room_alarm_callback, list_node);

        if (callback_in_list->timeout < timeout)
            break;
    }

    k_list_add(iter_node->prev, &callback->list_node);
    callback->impl.fn_del_self = alarm_callback_del_self;
    callback->data = data;
    callback->fn_callback = fn_callback;
    callback->timeout = timeout;

    return &callback->impl;
}
