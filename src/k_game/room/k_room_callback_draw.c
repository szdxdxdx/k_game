#include "k_game/alloc.h"

#include "./k_room_context.h"

void k__room_init_draw_callback_storage(struct k_room *room) {
    struct k_room_draw_callback_storage *storage = &room->draw_callbacks;

    k_list_init(&storage->z_lists);
}

void k__room_cleanup_draw_callback_storage(struct k_room *room) {
    struct k_room_draw_callback_storage *storage = &room->draw_callbacks;

    struct k_room_draw_callback_z_list *z_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(&storage->z_lists, iter, next)) {
        z_list = container_of(iter, struct k_room_draw_callback_z_list, list_node);

        struct k_room_draw_callback *callback;
        struct k_list_node *iter_, *next_;
        for (k_list_for_each_s(&z_list->callbacks_list, iter_, next_)) {
            callback = container_of(iter_, struct k_room_draw_callback, list_node);

            k_free(callback);
        }

        k_free(z_list);
    }
}

void k__room_exec_draw_callbacks(struct k_room *room) {
    struct k_room_draw_callback_storage *storage = &room->draw_callbacks;

    struct k_room_draw_callback_z_list *z_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(&storage->z_lists, iter, next)) {
        z_list = container_of(iter, struct k_room_draw_callback_z_list, list_node);

        struct k_room_draw_callback *callback;
        struct k_list_node *iter_, *next_;
        for (k_list_for_each_s(&z_list->callbacks_list, iter_, next_)) {
            callback = container_of(iter_, struct k_room_draw_callback, list_node);

            callback->fn_callback(callback->data);
        }
    }
}

static void draw_callback_del_self(struct k_room_callback *self) {
    struct k_room_draw_callback *callback = container_of(self, struct k_room_draw_callback, impl);

    struct k_room_draw_callback_z_list *z_list = callback->z_list;

    k_list_del(&callback->list_node);
    k_free(callback);

    if (k_list_is_empty(&z_list->callbacks_list)) {
        k_list_del(&z_list->list_node);
        k_free(z_list);
    }
}

struct k_room_callback *k__room_add_draw_callback(struct k_room *room, void (*fn_callback)(void *data), void *data, int z_index) {
    struct k_room_draw_callback_storage *storage = &room->draw_callbacks;

    struct k_room_draw_callback *callback = k_malloc(sizeof(struct k_room_draw_callback));
    if (NULL == callback)
        return NULL;

    struct k_room_draw_callback_z_list *z_list;
    struct k_list_node *iter;
    for (k_list_for_each(&storage->z_lists, iter)) {
        z_list = container_of(iter, struct k_room_draw_callback_z_list, list_node);

        if (z_index == z_list->z_index)
            goto add_callback;

        if (z_index < z_list->z_index)
            goto new_z_list;
    }

new_z_list:
    z_list = k_malloc(sizeof(struct k_room_draw_callback_z_list));
    if (NULL == z_list) {
        k_free(callback);
        return NULL;
    }

    z_list->z_index = z_index;
    k_list_init(&z_list->callbacks_list);
    k_list_add(iter->prev, &z_list->list_node);

add_callback:
    k_list_add_tail(&z_list->callbacks_list, &callback->list_node);
    callback->impl.fn_del_self = draw_callback_del_self;
    callback->z_list = z_list;
    callback->data = data;
    callback->fn_callback = fn_callback;

    return &callback->impl;
}
