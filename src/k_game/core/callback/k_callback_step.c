#include "./k_callback.h"

/* region [callback_def] */

struct k_step_callback {

    struct k_list_node list_node;

    struct k_callback base;
};

struct k_room_step_callback {

    struct k_step_callback step_callback; /* inherited */

    struct k_room_callback room_callback;

    void (*fn_callback)(void *data);

    void *data;
};

struct k_object_step_callback {

    struct k_step_callback step_callback; /* inherited */

    struct k_object_callback object_callback;

    void (*fn_callback)(struct k_object *object);

    struct k_object *object;
};

struct k_component_step_callback {

    struct k_step_callback step_callback; /* inherited */

    struct k_component_callback component_callback;

    void (*fn_callback)(struct k_component *component);

    struct k_component *component;
};

/* endregion */

void k__callback_init_step_manager(struct k_step_callback_manager *manager) {

    k_list_init(&manager->callback_list);
    k_list_init(&manager->pending_list);
}

void k__callback_deinit_step_manager(struct k_step_callback_manager *manager) {

    k__callback_flush_step(manager);

    struct k_step_callback *step_callback;
    struct k_list *list = &manager->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        step_callback = container_of(iter, struct k_step_callback, list_node);
        k_free(step_callback);
    }
}

void k__callback_flush_step(struct k_step_callback_manager *manager) {

    struct k_step_callback *callback;
    struct k_list *callback_list = &manager->callback_list;
    struct k_list *pending_list  = &manager->pending_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(pending_list, iter, next)) {
        callback = container_of(iter, struct k_step_callback, list_node);

        k_list_del(&callback->list_node);
        k_list_add_tail(callback_list, &callback->list_node);
    }
}

void k__callback_exec_step(struct k_step_callback_manager *manager) {

    struct k_step_callback *step_callback;
    struct k_list *callback_list = &manager->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_list, iter, next)) {
        step_callback = container_of(iter, struct k_step_callback, list_node);

        if (step_callback->base.is_deleted) {
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

struct k_room_callback *k__callback_add_room_step(struct k_step_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data)) {

    struct k_room_step_callback *callback = k_malloc(sizeof(struct k_room_step_callback));
    if (NULL == callback)
        return NULL;

    callback->step_callback.base.context = K_ROOM_CALLBACK;
    callback->step_callback.base.event   = K_STEP_CALLBACK;
    callback->step_callback.base.is_deleted = 0;

    k_list_add_tail(&manager->pending_list, &callback->step_callback.list_node);

    callback->room_callback.base = &callback->step_callback.base;
    k_list_add_tail(&room->callback_list, &callback->room_callback.list_node);

    callback->fn_callback = fn_callback;
    callback->data        = data;

    return &callback->room_callback;
}

struct k_object_callback *k__callback_add_object_step(struct k_step_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    struct k_object_step_callback *callback = k_malloc(sizeof(struct k_object_step_callback));
    if (NULL == callback)
        return NULL;

    callback->step_callback.base.context = K_OBJECT_CALLBACK;
    callback->step_callback.base.event   = K_STEP_CALLBACK;
    callback->step_callback.base.is_deleted = 0;

    k_list_add_tail(&manager->pending_list, &callback->step_callback.list_node);

    callback->object_callback.base = &callback->step_callback.base;
    k_list_add_tail(&object->callback_list, &callback->object_callback.list_node);

    callback->fn_callback = fn_callback;
    callback->object      = object;

    return &callback->object_callback;
}

struct k_component_callback *k__callback_add_component_step(struct k_step_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component)) {

    struct k_component_step_callback *callback = k_malloc(sizeof(struct k_component_step_callback));
    if (NULL == callback)
        return NULL;

    callback->step_callback.base.context = K_COMPONENT_CALLBACK;
    callback->step_callback.base.event   = K_STEP_CALLBACK;
    callback->step_callback.base.is_deleted = 0;

    k_list_add_tail(&manager->pending_list, &callback->step_callback.list_node);

    callback->component_callback.base = &callback->step_callback.base;
    k_list_add_tail(&component->callback_list, &callback->component_callback.list_node);

    callback->fn_callback = fn_callback;
    callback->component   = component;

    return &callback->component_callback;
}
