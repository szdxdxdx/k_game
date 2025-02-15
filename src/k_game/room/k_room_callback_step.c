#include "k_game/alloc.h"

#include "./k_room_callback_step.h"
#include "./k_room_context.h"

struct k_room_step_callback {

    struct k_room_callback impl;

    struct k_list_node list_node;

    void *data;

    void (*fn_callback)(void *data);
};

void k_room_init_step_callbacks_storage(struct k_room *room) {
    struct k_room_step_callbacks_storage *storage = &room->step_callbacks;

    k_list_init(&storage->list);
}

void k_room_clean_step_callbacks_storage(struct k_room *room) {
    struct k_room_step_callbacks_storage *storage = &room->step_callbacks;

    struct k_list_node *node, *next;
    for (k_list_for_each_s(&storage->list, node, next)) {
        struct k_room_step_callback *callback = container_of(node, struct k_room_step_callback, list_node);

        k_free(callback);
    }
}

void k_room_exec_step_callbacks(struct k_room *room) {
    struct k_room_step_callbacks_storage *storage = &room->step_callbacks;

    struct k_list_node *node, *next;
    for (k_list_for_each_s(&storage->list, node, next)) {
        struct k_room_step_callback *callback = container_of(node, struct k_room_step_callback, list_node);

        callback->fn_callback(callback->data);
    }
}

static void fn_del_self(struct k_room_callback *self) {
    struct k_room_step_callback *step_callback = container_of(self, struct k_room_step_callback, impl);

    k_list_del(&step_callback->list_node);
    k_free(step_callback);
}

struct k_room_callback *k_room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    struct k_room_step_callbacks_storage *storage = &room->step_callbacks;

    struct k_room_step_callback *callback = k_malloc(sizeof(struct k_room_step_callback));
    if (NULL == callback)
        return NULL;

    callback->impl.fn_del_self = fn_del_self;
    k_list_add_tail(&storage->list, &callback->list_node);
    callback->data = data;
    callback->fn_callback = fn_callback;

    return &callback->impl;
}
