#include "k_game/alloc.h"

#include "./k_room_context.h"
#include "../game/k_game_context.h"

void k__room_init_draw_callbacks_storage(struct k_room *room) {
    struct k_room_draw_callbacks_storage *storage = &room->draw_callbacks;

    k_list_init(&storage->depth_lists);
}

void k__room_del_all_draw_callbacks(struct k_room *room) {
    struct k_room_draw_callbacks_storage *storage = &room->draw_callbacks;

    struct k_room_draw_callback_depth_list *depth_list;
    struct k_room_draw_callback *callback;

    struct k_list_node *depth_iter, *next_;
    for (k_list_for_each_s(&storage->depth_lists, depth_iter, next_)) {
        depth_list = container_of(depth_iter, struct k_room_draw_callback_depth_list, list_node);

        struct k_list_node *callback_iter, *next;
        for (k_list_for_each_s(&depth_list->callbacks_list, callback_iter, next)) {
            callback = container_of(callback_iter, struct k_room_draw_callback, list_node);

            k_free(callback);
        }

        k_free(depth_list);
    }
}

void k__room_exec_draw_callbacks(struct k_room *room) {

    struct k_room_draw_callbacks_storage *storage = &room->draw_callbacks;

    struct k_room_draw_callback_depth_list *depth_list;
    struct k_room_draw_callback *callback;

    struct k_list_node *depth_iter, *next_;
    for (k_list_for_each_s(&storage->depth_lists, depth_iter, next_)) {
        depth_list = container_of(depth_iter, struct k_room_draw_callback_depth_list, list_node);

        struct k_list_node *callback_iter, *next;
        for (k_list_for_each_s(&depth_list->callbacks_list, callback_iter, next)) {
            callback = container_of(callback_iter, struct k_room_draw_callback, list_node);

            callback->fn_callback(callback->data);
        }
    }
}

static void draw_callback_del_self(struct k_room_callback *self) {
    struct k_room_draw_callback *callback = container_of(self, struct k_room_draw_callback, impl);

    struct k_room_draw_callback_depth_list *depth_list = callback->depth_list;

    k_list_del(&callback->list_node);
    k_free(callback);

    if (k_list_is_empty(&depth_list->callbacks_list))
        k_list_del(&depth_list->list_node);
}

struct k_room_callback *k_room_add_draw_callback(struct k_room *room, void (*fn_callback)(void *data), void *data, int depth) {
    struct k_room_draw_callbacks_storage *storage = &room->draw_callbacks;

    struct k_room_draw_callback *callback = k_malloc(sizeof(struct k_room_draw_callback));
    if (NULL == callback)
        return NULL;

    struct k_room_draw_callback_depth_list *depth_list = NULL;

    struct k_list_node *depth_iter;
    for (k_list_for_each(&storage->depth_lists, depth_iter)) {
        depth_list = container_of(depth_iter, struct k_room_draw_callback_depth_list, list_node);

        if (depth == depth_list->depth)
            goto add_callback;

        if (depth_list->depth < depth)
            break;
    }

    depth_list = k_malloc(sizeof(struct k_room_draw_callback_depth_list));
    if (NULL == depth_list) {
        k_free(callback);
        return NULL;
    }

    depth_list->depth = depth;
    k_list_init(&depth_list->callbacks_list);
    k_list_add(depth_iter->prev, &depth_list->list_node);

add_callback:
    k_list_add_tail(&depth_list->callbacks_list, &callback->list_node);
    callback->impl.fn_del_self = draw_callback_del_self;
    callback->depth_list = depth_list;
    callback->data = data;
    callback->fn_callback = fn_callback;

    return &callback->impl;
}
