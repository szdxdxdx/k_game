#include "k_game_alloc.h"
#include "k_game/room_context.h"

/* region [callback_list] */

void k__room_callback_list_init(struct k_room_callback_list *list) {
    k_list_init(&list->callback_list);
}

void k__room_callback_list_clean(struct k_room_callback_list *list) {

    struct k_room_callback_list_item *callback;
    struct k_list *callback_list = &list->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_list, iter, next)) {
        callback = container_of(iter, struct k_room_callback_list_item, list_node);

        k_free(callback);
    }
}

void k__room_callback_list_exec(struct k_room_callback_list *list) {

    struct k_room_callback_list_item *callback;
    struct k_list *callback_list = &list->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_list, iter, next)) {
        callback = container_of(iter, struct k_room_callback_list_item, list_node);

        if (callback->base.is_deleted) {
            k_list_del(&callback->list_node);
            k_free(callback);
        } else {
            callback->fn_callback(callback->data);
        }
    }
}

static inline struct k_room_callback *room_callback_list_add(struct k_room_callback_list *list, void (*fn_callback)(void *data), void *data) {

    struct k_room_callback_list_item *callback = k_malloc(sizeof(struct k_room_callback_list_item));
    if (NULL == callback)
        return NULL;

    callback->base.is_deleted = 0;
    callback->data = data;
    callback->fn_callback = fn_callback;
    k_list_add_tail(&list->callback_list, &callback->list_node);

    return &callback->base;
}

/* endregion */

/* region [use callback_list] */

struct k_room_callback *k__room_add_enter_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return room_callback_list_add(&room->enter_callbacks, fn_callback, data);
}

struct k_room_callback *k__room_add_leave_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    return room_callback_list_add(&room->leave_callbacks, fn_callback, data);
}

/* endregion */
