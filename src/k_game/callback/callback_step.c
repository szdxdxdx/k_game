#include "k_game_alloc.h"

#include "./callback.h"

void k__init_step_callback_manager(struct k_step_callback_manager *manager) {

    k_list_init(&manager->callback_list);
    k_list_init(&manager->pending_list);
}

void k__deinit_step_callback_manager(struct k_step_callback_manager *manager) {

    struct k_step_callback *step_callback;
    struct k_list *list;
    struct k_list_node *iter, *next;

    list = &manager->callback_list;
    for (k_list_for_each_s(list, iter, next)) {
        step_callback = container_of(iter, struct k_step_callback, list_node);
        k_free(step_callback);
    }

    list = &manager->pending_list;
    for (k_list_for_each_s(list, iter, next)) {
        step_callback = container_of(iter, struct k_step_callback, list_node);
        k_free(step_callback);
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

        switch (step_callback->base.context) {
            case K_ROOM_CALLBACK: {
                struct k_room_step_callback *callback = (struct k_room_step_callback *)step_callback;
                callback->fn_callback(callback->data);
                break;
            }
            case K_OBJECT_CALLBACK: {
                struct k_object_step_callback *callback = (struct k_object_step_callback *)step_callback;
                callback->fn_callback(callback->object);
                break;
            }
            case K_COMPONENT_CALLBACK: {
                struct k_component_step_callback *callback = (struct k_component_step_callback *)step_callback;
                callback->fn_callback(callback->component);
                break;
            }
        }
    }
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

struct k_component_callback *k__add_component_step_callback(struct k_step_callback_manager *manager, void (*fn_callback)(struct k_component *component), struct k_component *component) {

    struct k_component_step_callback *callback = k_malloc(sizeof(struct k_component_step_callback));
    if (NULL == callback)
        return NULL;

    callback->step_callback.base.event   = K_STEP_CALLBACK;
    callback->step_callback.base.context = K_COMPONENT_CALLBACK;
    callback->step_callback.base.deleted = 0;
    callback->component_callback.base    = &callback->step_callback.base;
    callback->fn_callback                = fn_callback;
    callback->component                  = component;
    k_list_add_tail(&manager->pending_list, &callback->step_callback.list_node);

    return &callback->component_callback;
}
