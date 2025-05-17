#include <assert.h>

#include "k_game/core/k_mem_alloc.h"

#include "./k_callback_step.h"

#include "../room/k_room.h"
#include "../object/k_object.h"
#include "../component/k_component.h"
#include "../component/k_component_manager.h"

/* region [callback_manager] */

/* region [def] */

enum k_step_callback_context {
    K__STEP_CALLBACK_CONTEXT_ROOM,
    K__STEP_CALLBACK_CONTEXT_OBJECT,
    K__STEP_CALLBACK_CONTEXT_COMPONENT,
    K__STEP_CALLBACK_CONTEXT_COMPONENT_MANAGER,
};

enum k_step_callback_state {
    K__STEP_CALLBACK_STATE_INACTIVE,
    K__STEP_CALLBACK_STATE_ACTIVE,
    K__STEP_CALLBACK_STATE_DELETED,
};

struct k_step_callback {

    struct k_list_node callback_list_node;
    struct k_list_node pending_list_node;

    struct k_step_callback_manager *manager;

    struct k_callback base;

    enum k_step_callback_state state;

    enum k_step_callback_context context;

    union {
        void (*fn_room_callback)(void *data);
        void (*fn_object_callback)(struct k_object *object);
        void (*fn_component_callback)(struct k_component *component);
        void (*fn_component_manager_callback)(void *data);
    };

    union {
        void *data;
        struct k_object *object;
        struct k_component *component;
    };
};

/* endregion */

/* region [del] */

static void k__step_callback_manager_del_callback(struct k_callback *callback) {
    struct k_step_callback *step_callback = container_of(callback, struct k_step_callback, base);

    switch (step_callback->state) {
        case K__STEP_CALLBACK_STATE_INACTIVE:
            step_callback->state = K__STEP_CALLBACK_STATE_DELETED;
            break;
        case K__STEP_CALLBACK_STATE_ACTIVE:
            k_list_insert_tail(&step_callback->manager->pending_list, &step_callback->pending_list_node);
            step_callback->state =K__STEP_CALLBACK_STATE_DELETED;
            break;
        case K__STEP_CALLBACK_STATE_DELETED:
            return;
        default:
            assert(0);
    }

    k_list_remove(&step_callback->base.context_callback_list_node);
}

/* endregion */

/* region [add] */

static struct k_callback *k__step_callback_manager_add_room_callback(struct k_step_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data)) {

    struct k_step_callback *callback = k__mem_alloc(sizeof(struct k_step_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__step_callback_manager_del_callback;

    callback->manager = manager;
    callback->context = K__STEP_CALLBACK_CONTEXT_ROOM;
    callback->state   = K__STEP_CALLBACK_STATE_INACTIVE;

    k_list_insert_tail(&manager->pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_room_callback = fn_callback;
    callback->data = data;
    k_list_insert_tail(&room->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

static struct k_callback *k__step_callback_manager_add_object_callback(struct k_step_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    struct k_step_callback *callback = k__mem_alloc(sizeof(struct k_step_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__step_callback_manager_del_callback;

    callback->manager = manager;
    callback->context = K__STEP_CALLBACK_CONTEXT_OBJECT;
    callback->state   = K__STEP_CALLBACK_STATE_INACTIVE;

    k_list_insert_tail(&manager->pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_object_callback = fn_callback;
    callback->object = object;
    k_list_insert_tail(&object->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

static struct k_callback *k__step_callback_manager_add_component_callback(struct k_step_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component)) {

    struct k_step_callback *callback = k__mem_alloc(sizeof(struct k_step_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__step_callback_manager_del_callback;

    callback->manager = manager;
    callback->context = K__STEP_CALLBACK_CONTEXT_COMPONENT;
    callback->state   = K__STEP_CALLBACK_STATE_INACTIVE;

    k_list_insert_tail(&manager->pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_component_callback = fn_callback;
    callback->component = component;
    k_list_insert_tail(&component->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

static struct k_callback *k__step_callback_manager_add_component_manager_callback(struct k_step_callback_manager *manager, struct k_component_manager *component_manager, void *data, void (*fn_callback)(void *data)) {

    struct k_step_callback *callback = k__mem_alloc(sizeof(struct k_step_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__step_callback_manager_del_callback;

    callback->manager = manager;
    callback->context = K__STEP_CALLBACK_CONTEXT_COMPONENT_MANAGER;
    callback->state   = K__STEP_CALLBACK_STATE_INACTIVE;

    callback->manager = manager;
    k_list_insert_tail(&manager->pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_component_manager_callback = fn_callback;
    callback->data = data;
    k_list_insert_tail(&component_manager->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

/* endregion */

/* region [init] */

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

        k__mem_free(callback);
    }
}

/* endregion */

/* region [exec] */

void k__step_callback_manager_flush(struct k_step_callback_manager *manager) {

    struct k_step_callback *callback;
    struct k_list *callback_list = &manager->callback_list;
    struct k_list *pending_list  = &manager->pending_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(pending_list, iter, next)) {
        callback = container_of(iter, struct k_step_callback, pending_list_node);

        switch (callback->state) {
            case K__STEP_CALLBACK_STATE_INACTIVE:
                k_list_remove(&callback->pending_list_node);
                k_list_node_loop(&callback->pending_list_node);
                k_list_insert_tail(callback_list, &callback->callback_list_node);
                callback->state = K__STEP_CALLBACK_STATE_ACTIVE;
                break;
            case K__STEP_CALLBACK_STATE_DELETED:
                k_list_remove(&callback->callback_list_node);
                k_list_remove(&callback->pending_list_node);
                k__mem_free(callback);
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

        if (K__STEP_CALLBACK_STATE_ACTIVE != callback->state)
            continue;

        switch (callback->context) {
            case K__STEP_CALLBACK_CONTEXT_ROOM:
                callback->fn_room_callback(callback->data);
                break;
            case K__STEP_CALLBACK_CONTEXT_OBJECT:
                callback->fn_object_callback(callback->object);
                break;
            case K__STEP_CALLBACK_CONTEXT_COMPONENT:
                callback->fn_component_callback(callback->component);
                break;
            case K__STEP_CALLBACK_CONTEXT_COMPONENT_MANAGER:
                callback->fn_component_manager_callback(callback->data);
                break;
            default:
                assert(0);
        }
    }
}

/* endregion */

/* endregion */

/* region [add_callback] */

/* region [room_add_step_callback] */

struct k_callback *k_room_add_begin_step_callback(void *data, void (*fn_callback)(void *data)) {
    struct k_room *room = k__room_current;
    return k__step_callback_manager_add_room_callback(&room->begin_step_callback_manager, room, data, fn_callback);
}

struct k_callback *k_room_add_step_callback(void *data, void (*fn_callback)(void *data)) {
    struct k_room *room = k__room_current;
    return k__step_callback_manager_add_room_callback(&room->step_callback_manager, room, data, fn_callback);
}

struct k_callback *k_room_add_end_step_callback(void *data, void (*fn_callback)(void *data)) {
    struct k_room *room = k__room_current;
    return k__step_callback_manager_add_room_callback(&room->end_step_callback_manager, room, data, fn_callback);
}

/* endregion */

/* region [object_add_callback] */

struct k_callback *k_object_add_begin_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {
    return k__step_callback_manager_add_object_callback(&object->room->begin_step_callback_manager, object, fn_callback);
}

struct k_callback *k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {
    return k__step_callback_manager_add_object_callback(&object->room->step_callback_manager, object, fn_callback);
}

struct k_callback *k_object_add_end_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {
    return k__step_callback_manager_add_object_callback(&object->room->end_step_callback_manager, object, fn_callback);
}

/* endregion */

/* region [component_add_callback] */

struct k_callback *k_component_add_begin_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {
    return k__step_callback_manager_add_component_callback(&component->object->room->begin_step_callback_manager, component, fn_callback);
}

struct k_callback *k_component_add_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {
    return k__step_callback_manager_add_component_callback(&component->object->room->step_callback_manager, component, fn_callback);
}

struct k_callback *k_component_add_end_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {
    return k__step_callback_manager_add_component_callback(&component->object->room->end_step_callback_manager, component, fn_callback);
}

/* endregion */

/* region [component_manager_add_callback] */

struct k_callback *k_component_manager_add_begin_step_callback(struct k_component_manager *manager, void *data, void (*fn_callback)(void *data)) {
    return k__step_callback_manager_add_component_manager_callback(&manager->room->begin_step_callback_manager, manager, data, fn_callback);
}

struct k_callback *k_component_manager_add_step_callback(struct k_component_manager *manager, void *data, void (*fn_callback)(void *data)) {
    return k__step_callback_manager_add_component_manager_callback(&manager->room->step_callback_manager, manager, data, fn_callback);
}

struct k_callback *k_component_manager_add_end_step_callback(struct k_component_manager *manager, void *data, void (*fn_callback)(void *data)) {
    return k__step_callback_manager_add_component_manager_callback(&manager->room->step_callback_manager, manager, data, fn_callback);
}

/* endregion */

/* endregion */
