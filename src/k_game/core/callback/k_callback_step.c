#include <assert.h>

#include "k_game/core/k_mem_alloc.h"

#include "./k_callback_step.h"

#include "../room/k_room.h"
#include "../object/k_object.h"
#include "../component/k_component.h"
#include "../component/k_component_manager.h"

/* region [callback_def] */

struct k_step_callback {

    struct k_list_node callback_list_node;
    struct k_list_node pending_list_node;

    struct k_callback base;

    struct k_step_callback_manager *manager;

    union {
        void (*fn_room_callback)(void *data);
        void (*fn_object_callback)(struct k_object *object);
        void (*fn_component_callback)(struct k_component *component);
        void (*fn_component_manager_callback)(struct k_component_manager *component_manager);
    };

    union {
        void *data;
        struct k_object *object;
        struct k_component *component;
        struct k_component_manager *component_manager;
    };
};

/* endregion */

/* region [add_callback] */

struct k_callback *k__step_callback_manager_add_room_callback(struct k_step_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data)) {

    struct k_step_callback *callback = k_mem_alloc(sizeof(struct k_step_callback));
    if (NULL == callback)
        return NULL;

    callback->base.context = K__ROOM_CALLBACK;
    callback->base.event   = K__STEP_CALLBACK;
    callback->base.state   = K__CALLBACK_INACTIVE;

    callback->manager = manager;
    k_list_add_tail(&manager->pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_room_callback = fn_callback;
    callback->data = data;
    k_list_add_tail(&room->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

struct k_callback *k__step_callback_manager_add_object_callback(struct k_step_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    struct k_step_callback *callback = k_mem_alloc(sizeof(struct k_step_callback));
    if (NULL == callback)
        return NULL;

    callback->base.context = K__OBJECT_CALLBACK;
    callback->base.event   = K__STEP_CALLBACK;
    callback->base.state   = K__CALLBACK_INACTIVE;

    callback->manager = manager;
    k_list_add_tail(&manager->pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);


    callback->fn_object_callback = fn_callback;
    callback->object = object;
    k_list_add_tail(&object->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

struct k_callback *k__step_callback_manager_add_component_callback(struct k_step_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component)) {

    struct k_step_callback *callback = k_mem_alloc(sizeof(struct k_step_callback));
    if (NULL == callback)
        return NULL;

    callback->base.context = K__COMPONENT_CALLBACK;
    callback->base.event   = K__STEP_CALLBACK;
    callback->base.state   = K__CALLBACK_INACTIVE;

    callback->manager = manager;
    k_list_add_tail(&manager->pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);


    callback->fn_component_callback = fn_callback;
    callback->component = component;
    k_list_add_tail(&component->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

struct k_callback *k__step_callback_manager_add_component_manager_callback(struct k_step_callback_manager *manager, struct k_component_manager *component_manager, void (*fn_callback)(struct k_component_manager *component_manager)) {

    assert(0); /* experimental */

    struct k_step_callback *callback = k_mem_alloc(sizeof(struct k_step_callback));
    if (NULL == callback)
        return NULL;

    callback->base.context = K__COMPONENT_MANAGER_CALLBACK;
    callback->base.event   = K__STEP_CALLBACK;
    callback->base.state   = K__CALLBACK_INACTIVE;

    callback->manager = manager;
    k_list_add_tail(&manager->pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_component_manager_callback = fn_callback;
    callback->component_manager = component_manager;
    k_list_add_tail(&component_manager->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

/* endregion */

/* region [del_callback] */

void k__step_callback_manager_del_callback(struct k_callback *callback) {

    if (K__CALLBACK_DELETED == callback->state)
        return;

    struct k_step_callback *step_callback = container_of(callback, struct k_step_callback, base);

    switch (callback->state) {
        case K__CALLBACK_INACTIVE:
            callback->state = K__CALLBACK_DELETED;
            break;
        case K__CALLBACK_ACTIVE:
            k_list_add_tail(&step_callback->manager->pending_list, &step_callback->pending_list_node);
            callback->state = K__CALLBACK_DELETED;
            break;
        default:
            assert(0);
    }

    k_list_del(&step_callback->base.context_callback_list_node);
}

/* endregion */

/* region [callback_manager] */

void k__step_callback_manager_init(struct k_step_callback_manager *manager) {

    k_list_init(&manager->callback_list);
    k_list_init(&manager->pending_list);
}

void k__step_callback_manager_deinit(struct k_step_callback_manager *manager) {

    k__step_callback_manager_flush(manager);

    struct k_step_callback *callback;
    struct k_list *list = &manager->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_step_callback, callback_list_node);

        k_mem_free(callback);
    }
}

void k__step_callback_manager_flush(struct k_step_callback_manager *manager) {

    struct k_step_callback *callback;
    struct k_list *callback_list = &manager->callback_list;
    struct k_list *pending_list  = &manager->pending_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(pending_list, iter, next)) {
        callback = container_of(iter, struct k_step_callback, pending_list_node);

        switch (callback->base.state) {
            case K__CALLBACK_INACTIVE:
                k_list_del(&callback->pending_list_node);
                k_list_node_loop(&callback->pending_list_node);
                k_list_add_tail(callback_list, &callback->callback_list_node);
                callback->base.state = K__CALLBACK_ACTIVE;
                break;
            case K__CALLBACK_DELETED:
                k_list_del(&callback->callback_list_node);
                k_list_del(&callback->pending_list_node);
                k_mem_free(callback);
                break;
            default:
                assert(0);
        }
    }
}

void k__step_callback_manager_exec(struct k_step_callback_manager *manager) {

    struct k_step_callback *callback;
    struct k_list *callback_list = &manager->callback_list;
    struct k_list_node *iter;
    for (k_list_for_each(callback_list, iter)) {
        callback = container_of(iter, struct k_step_callback, callback_list_node);

        if (K__CALLBACK_ACTIVE != callback->base.state)
            continue;

        switch (callback->base.context) {
            case K__ROOM_CALLBACK:
                callback->fn_room_callback(callback->data);
                break;
            case K__OBJECT_CALLBACK:
                callback->fn_object_callback(callback->object);
                break;
            case K__COMPONENT_CALLBACK:
                callback->fn_component_callback(callback->component);
                break;
            case K__COMPONENT_MANAGER_CALLBACK:
                assert(0); /* experimental */
                callback->fn_component_manager_callback(callback->component_manager);
                break;
            default:
                assert(0);
        }
    }
}

/* endregion */
