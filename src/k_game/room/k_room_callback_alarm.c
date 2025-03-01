#include "k_game/alloc.h"

#include "../game/k_game_context.h"
#include "./k_room_context.h"

void k__room_init_alarm_callback_storage(struct k_room *room) {
    struct k_room_alarm_callback_storage *storage = &room->alarm_callbacks;

    k_list_init(&storage->list);
}

void k__room_cleanup_alarm_callback_storage(struct k_room *room) {

    struct k_list *callback_list = &room->alarm_callbacks.list;
    struct k_room_alarm_callback *callback;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_list, iter, next)) {
        callback = container_of(iter, struct k_room_alarm_callback, list_node);

        k_free(callback);
    }
}

void k__room_exec_alarm_callbacks(struct k_room *room) {
    struct k_room_alarm_callback_storage *storage = &room->alarm_callbacks;

    /* [?] 应该使用当前时间，还是当前帧时间 */
    const uint64_t current_ms = k__game.step_timestamp;

    struct k_list *callback_list = &storage->list;
    struct k_room_alarm_callback *callback;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_list, iter, next)) {
        callback = container_of(iter, struct k_room_alarm_callback, list_node);

        if (callback->timeout <= current_ms) {
            int timeout_diff = (int)(current_ms - callback->timeout);

            /* [?] alarm_callback 结点应何时删除
             * [?] alarm callback 不应该用 `k_room_del_callback()` 删除自身
             */

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

struct k_room_callback *k__room_add_alarm_callback(struct k_room *room, void (*fn_callback)(void *data, int timeout_diff), void *data, int delay_ms) {
    struct k_room_alarm_callback_storage *storage = &room->alarm_callbacks;

    if (delay_ms < 0)
        delay_ms = 0;

    struct k_room_alarm_callback *callback = k_malloc(sizeof(struct k_room_alarm_callback));
    if (NULL == callback)
        return NULL;

    uint64_t timeout = k__game.step_timestamp + delay_ms;

    struct k_list *callback_list = &storage->list;
    struct k_room_alarm_callback *callback_in_list;
    struct k_list_node *iter;
    for (k_list_for_each(callback_list, iter)) {
        callback_in_list = container_of(iter, struct k_room_alarm_callback, list_node);

        if (callback_in_list->timeout < timeout)
            break;
    }

    k_list_add(iter->prev, &callback->list_node);
    callback->impl.fn_del_self = alarm_callback_del_self;
    callback->data = data;
    callback->fn_callback = fn_callback;
    callback->timeout = timeout;

    return &callback->impl;
}
