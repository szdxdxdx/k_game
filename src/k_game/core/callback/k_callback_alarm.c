#include <assert.h>

#include "./_internal.h"

/* region [callback_def] */

struct k_alarm_callback {

    struct k_list_node callback_list_node;
    struct k_list_node pending_list_node;

    struct k_callback_base base;

    uint64_t timeout;

    struct k_alarm_callback_manager *manager;
};

struct k_room_alarm_callback {

    struct k_alarm_callback alarm_callback; /* inherited */

    struct k_room_callback room_callback;

    void (*fn_callback)(void *data, int timeout_diff);

    void *data;
};

struct k_object_alarm_callback {

    struct k_alarm_callback alarm_callback; /* inherited */

    struct k_object_callback object_callback;

    void (*fn_callback)(struct k_object *object, int timeout_diff);

    struct k_object *object;
};

struct k_component_alarm_callback {

    struct k_alarm_callback alarm_callback; /* inherited */

    struct k_component_callback component_callback;

    void (*fn_callback)(struct k_component *component, int timeout_diff);

    struct k_component *component;
};

/* endregion */

/* region [add_callback] */

struct k_room_callback *k__callback_add_room_alarm(struct k_alarm_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms) {

    struct k_room_alarm_callback *callback = k_malloc(sizeof(struct k_room_alarm_callback));
    if (NULL == callback)
        return NULL;

    /* [?] 应该使用当前时间，还是当前帧时间 */
    uint64_t timeout = k_get_step_timestamp() + delay_ms;

    callback->alarm_callback.base.context = K_ROOM_CALLBACK;
    callback->alarm_callback.base.event   = K_ALARM_CALLBACK;
    callback->alarm_callback.base.state   = K_CALLBACK_PENDING;

    callback->alarm_callback.timeout = timeout;
    callback->alarm_callback.manager = manager;
    k_list_add_tail(&manager->pending_list, &callback->alarm_callback.pending_list_node);
    k_list_node_loop(&callback->alarm_callback.callback_list_node);

    callback->room_callback.base = &callback->alarm_callback.base;
    k_list_add_tail(&room->callback_list, &callback->room_callback.list_node);

    callback->fn_callback = fn_callback;
    callback->data        = data;

    return &callback->room_callback;
}

struct k_object_callback *k__callback_add_object_alarm(struct k_alarm_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms) {

    struct k_object_alarm_callback *callback = k_malloc(sizeof(struct k_object_alarm_callback));
    if (NULL == callback)
        return NULL;

    /* [?] 应该使用当前时间，还是当前帧时间 */
    uint64_t timeout = k_get_step_timestamp() + delay_ms;

    callback->alarm_callback.base.context = K_OBJECT_CALLBACK;
    callback->alarm_callback.base.event   = K_ALARM_CALLBACK;
    callback->alarm_callback.base.state   = K_CALLBACK_PENDING;

    callback->alarm_callback.timeout = timeout;
    callback->alarm_callback.manager = manager;
    k_list_add_tail(&manager->pending_list, &callback->alarm_callback.pending_list_node);
    k_list_node_loop(&callback->alarm_callback.callback_list_node);

    callback->object_callback.base = &callback->alarm_callback.base;
    k_list_add_tail(&object->callback_list, &callback->object_callback.list_node);

    callback->fn_callback = fn_callback;
    callback->object      = object;

    return &callback->object_callback;
}

struct k_component_callback *k__callback_add_component_alarm(struct k_alarm_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms) {

    struct k_component_alarm_callback *callback = k_malloc(sizeof(struct k_component_alarm_callback));
    if (NULL == callback)
        return NULL;

    /* [?] 应该使用当前时间，还是当前帧时间 */
    uint64_t timeout = k_get_step_timestamp() + delay_ms;

    callback->alarm_callback.base.context = K_COMPONENT_CALLBACK;
    callback->alarm_callback.base.event   = K_ALARM_CALLBACK;
    callback->alarm_callback.base.state   = K_CALLBACK_PENDING;

    callback->alarm_callback.timeout = timeout;
    callback->alarm_callback.manager = manager;
    k_list_add_tail(&manager->pending_list, &callback->alarm_callback.pending_list_node);
    k_list_node_loop(&callback->alarm_callback.callback_list_node);

    callback->component_callback.base = &callback->alarm_callback.base;
    k_list_add_tail(&component->callback_list, &callback->component_callback.list_node);

    callback->fn_callback = fn_callback;
    callback->component   = component;

    return &callback->component_callback;
}

/* endregion */

/* region [del_callback] */

static void free_alarm_callback(struct k_alarm_callback *alarm_callback) {

    k_list_del(&alarm_callback->callback_list_node);
    k_list_del(&alarm_callback->pending_list_node);

    switch (alarm_callback->base.context) {
        case K_ROOM_CALLBACK: {
            struct k_room_alarm_callback *callback = (struct k_room_alarm_callback *)alarm_callback;
            k_list_del(&callback->room_callback.list_node);
            break;
        }
        case K_OBJECT_CALLBACK: {
            struct k_object_alarm_callback *callback = (struct k_object_alarm_callback *)alarm_callback;
            k_list_del(&callback->object_callback.list_node);
            break;
        }
        case K_COMPONENT_CALLBACK: {
            struct k_component_alarm_callback *callback = (struct k_component_alarm_callback *)alarm_callback;
            k_list_del(&callback->component_callback.list_node);
            break;
        }
    }

    k_free(alarm_callback);
}

void k__callback_flag_deleted_alarm(struct k_callback_base *callback) {

    struct k_alarm_callback *alarm_callback = container_of(callback, struct k_alarm_callback, base);

    switch (alarm_callback->base.state)
        case K_CALLBACK_PENDING: {
            alarm_callback->base.state = K_CALLBACK_DELETED;
            break;
        case K_CALLBACK_ACTIVE:
            k_list_add_tail(&alarm_callback->manager->pending_list, &alarm_callback->pending_list_node);
            alarm_callback->base.state = K_CALLBACK_DELETED;
            break;
        case K_CALLBACK_EXECUTED:
            alarm_callback->base.state = K_CALLBACK_DELETED;
            break;
        default:
            assert(0);
    }
}

void k__callback_flag_dead_alarm(struct k_callback_base *callback) {

    struct k_alarm_callback *alarm_callback = container_of(callback, struct k_alarm_callback, base);

    switch (alarm_callback->base.state)
        case K_CALLBACK_PENDING: {
            alarm_callback->base.state = K_CALLBACK_DEAD;
            break;
        case K_CALLBACK_ACTIVE:
            k_list_add_tail(&alarm_callback->manager->pending_list, &alarm_callback->pending_list_node);
            alarm_callback->base.state = K_CALLBACK_DEAD;
            break;
        case K_CALLBACK_EXECUTED:
        case K_CALLBACK_DELETED:
            alarm_callback->base.state = K_CALLBACK_DEAD;
            break;
        default:
            assert(0);
    }
}

/* endregion */

/* region [callback_manager] */

void k__callback_init_alarm_manager(struct k_alarm_callback_manager *manager) {
    k_list_init(&manager->callback_list);
    k_list_init(&manager->pending_list);
}

void k__callback_deinit_alarm_manager(struct k_alarm_callback_manager *manager) {

    k__callback_flush_alarm(manager);

    struct k_alarm_callback *alarm_callback;
    struct k_list *list = &manager->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        alarm_callback = container_of(iter, struct k_alarm_callback, callback_list_node);
        k_free(alarm_callback);
    }
}

void k__callback_flush_alarm(struct k_alarm_callback_manager *manager) {

    struct k_alarm_callback *alarm_callback;
    struct k_list *list = &manager->pending_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        alarm_callback = container_of(iter, struct k_alarm_callback, pending_list_node);


        switch (alarm_callback->base.state) {
            case K_CALLBACK_PENDING:
                k_list_del(&alarm_callback->pending_list_node);
                k_list_node_loop(&alarm_callback->pending_list_node);

                /* TODO 改用优先队列后，修改这个代码块 */
                {
                    uint64_t timeout = alarm_callback->timeout;

                    struct k_alarm_callback *callback_in_list;
                    struct k_list *callback_list = &manager->callback_list;
                    struct k_list_node *iter_;
                    for (k_list_for_each(callback_list, iter_)) {
                        callback_in_list = container_of(iter_, struct k_alarm_callback, callback_list_node);

                        if (timeout <= callback_in_list->timeout)
                            break;
                    }
                    k_list_add(iter_->prev, &alarm_callback->callback_list_node);
                }
                /* TODO 改用优先队列后，修改这个代码块 */

                alarm_callback->base.state = K_CALLBACK_ACTIVE;
                break;
            case K_CALLBACK_EXECUTED:
            case K_CALLBACK_DELETED:
            case K_CALLBACK_DEAD:
                free_alarm_callback(alarm_callback);
                break;
            default:
                assert(0);
        }
    }
}

void k__callback_exec_alarm(struct k_alarm_callback_manager *manager) {

    /* [?] 应该使用当前时间，还是当前帧时间 */
    const uint64_t current_ms = k_get_step_timestamp();

    struct k_list *callback_list = &manager->callback_list;
    struct k_alarm_callback *alarm_callback;
    struct k_list_node *iter;
    for (k_list_for_each(callback_list, iter)) {
        alarm_callback = container_of(iter, struct k_alarm_callback, callback_list_node);

        if (current_ms < alarm_callback->timeout)
            break;

        if (K_CALLBACK_DEAD == alarm_callback->base.state)
            continue;

        int timeout_diff = (int)(current_ms - alarm_callback->timeout);

        k_list_add_tail(&manager->pending_list, &alarm_callback->pending_list_node);
        alarm_callback->base.state = K_CALLBACK_EXECUTED;

        switch (alarm_callback->base.context) {
            case K_ROOM_CALLBACK: {
                struct k_room_alarm_callback *callback = (struct k_room_alarm_callback *)alarm_callback;
                callback->fn_callback(callback->data, timeout_diff);
                break;
            }
            case K_OBJECT_CALLBACK: {
                struct k_object_alarm_callback *callback = (struct k_object_alarm_callback *)alarm_callback;
                callback->fn_callback(callback->object, timeout_diff);
                break;
            }
            case K_COMPONENT_CALLBACK: {
                struct k_component_alarm_callback *callback = (struct k_component_alarm_callback *)alarm_callback;
                callback->fn_callback(callback->component, timeout_diff);
                break;
            }
        }
    }
}

/* endregion */
