#include "k_game_alloc.h"

#include "./callback.h"

void k__init_step_callback_manager(struct k_step_callback_manager *manager) {

    k_list_init(&manager->callback_list);
    k_list_init(&manager->pending_list);
}

void k__deinit_step_callback_manager(struct k_step_callback_manager *manager) {

    /* TODO */
}

struct k_room_callback *k__add_room_step_callback(struct k_step_callback_manager *manager, void (*fn_callback)(void *data), void *data) {

    struct k_room_step_callback *callback = k_malloc(sizeof(struct k_room_step_callback));
    if (NULL == callback)
        return NULL;

    callback->step_callback.base.event   = K_STEP_CALLBACK;
    callback->step_callback.base.context = K_ROOM_CALLBACK;
    callback->step_callback.base.deleted = 0;
    callback->room_callback.base         = &callback->step_callback.base;
    callback->fn_callback                = fn_callback;
    callback->data                       = data;
    k_list_add_tail(&manager->pending_list, &callback->step_callback.list_node);

    return &callback->room_callback;
}

struct k_object_callback *k__add_object_step_callback(struct k_step_callback_manager *manager, void (*fn_callback)(struct k_object *object), struct k_object *object) {

    struct k_object_step_callback *callback = k_malloc(sizeof(struct k_object_step_callback));
    if (NULL == callback)
        return NULL;

    callback->step_callback.base.event   = K_STEP_CALLBACK;
    callback->step_callback.base.context = K_OBJECT_CALLBACK;
    callback->step_callback.base.deleted = 0;
    callback->object_callback.base       = &callback->step_callback.base;
    callback->fn_callback                = fn_callback;
    callback->object                     = object;
    k_list_add_tail(&manager->pending_list, &callback->step_callback.list_node);

    return &callback->object_callback;
}

void k__exec_step_callbacks(struct k_step_callback_manager *manager) {

    struct k_step_callback *step_callback;
    struct k_list *callback_list = &manager->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_list, iter, next)) {
        step_callback = container_of(iter, struct k_step_callback, list_node);

        if (step_callback->base.deleted) {
            k_list_del(&step_callback->list_node);
            k_free(step_callback);
            continue;
        }

        if (K_ROOM_CALLBACK == step_callback->base.context) {
            struct k_room_step_callback *callback = container_of(step_callback, struct k_room_step_callback, step_callback);
            callback->fn_callback(callback->data);
        }
        else if (K_OBJECT_CALLBACK == step_callback->base.context) {
            struct k_object_step_callback *callback = container_of(step_callback, struct k_object_step_callback, step_callback);
            callback->fn_callback(callback->object);
        }
    }
}

void k__flush_step_callbacks(struct k_step_callback_manager *manager) {

    struct k_list *callback_list = &manager->callback_list;
    struct k_list *pending_list  = &manager->pending_list;

    struct k_step_callback *callback;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(pending_list, iter, next)) {
        callback = container_of(iter, struct k_step_callback, list_node);

        k_list_del(&callback->list_node);
        k_list_add_tail(callback_list, &callback->list_node);
    }
}
