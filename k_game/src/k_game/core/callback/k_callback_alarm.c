#include <assert.h>

#include "k_game/core/k_mem_alloc.h"
#include "k_game/core/k_time.h"

#include "./k_callback_alarm.h"

#include "../room/k_room.h"
#include "../object/k_object.h"
#include "../component/k_component.h"
#include "../component/k_component_manager.h"

/* region [callback_manager] */

/* region [def] */

enum k_alarm_callback_context {
    K__ALARM_CALLBACK_CONTEXT_ROOM,
    K__ALARM_CALLBACK_CONTEXT_OBJECT,
    K__ALARM_CALLBACK_CONTEXT_COMPONENT,
    K__ALARM_CALLBACK_CONTEXT_COMPONENT_MANAGER,
};

enum k_alarm_callback_state {
    K__ALARM_CALLBACK_STATE_INACTIVE,
    K__ALARM_CALLBACK_STATE_ACTIVE,
    K__ALARM_CALLBACK_STATE_EXECUTED,
    K__ALARM_CALLBACK_STATE_DELETED,
};

struct k_alarm_callback {

    struct k_list_node callback_list_node;
    struct k_list_node pending_list_node;

    struct k_alarm_callback_manager *manager;

    struct k_callback base;

    enum k_alarm_callback_state state;

    enum k_alarm_callback_context context;

    uint64_t timeout;

    union {
        void (*fn_room_callback)(void *data, int timeout_diff);
        void (*fn_object_callback)(struct k_object *object, int timeout_diff);
        void (*fn_component_callback)(struct k_component *component, int timeout_diff);
        void (*fn_component_manager_callback)(void *data, int timeout_diff);
    };

    union {
        void *data;
        struct k_object *object;
        struct k_component *component;
    };
};

/* endregion */

/* region [del] */

void k__alarm_callback_manager_del_callback(struct k_callback *callback) {

    struct k_alarm_callback *alarm_callback = container_of(callback, struct k_alarm_callback, base);

    switch (alarm_callback->state)
        case K__ALARM_CALLBACK_STATE_INACTIVE: {
            alarm_callback->state = K__ALARM_CALLBACK_STATE_DELETED;
            break;
        case K__ALARM_CALLBACK_STATE_ACTIVE:
            k_list_insert_tail(&alarm_callback->manager->pending_list, &alarm_callback->pending_list_node);
            alarm_callback->state = K__ALARM_CALLBACK_STATE_DELETED;
            break;
        case K__ALARM_CALLBACK_STATE_EXECUTED:
            alarm_callback->state = K__ALARM_CALLBACK_STATE_DELETED;
            break;
        case K__ALARM_CALLBACK_STATE_DELETED:
                return;
        default:
            assert(0);
    }

    k_list_remove(&alarm_callback->base.context_callback_list_node);
}

/* endregion */

/* region [add] */

struct k_callback *k__alarm_callback_manager_add_room_callback(struct k_alarm_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms) {

    struct k_alarm_callback *callback = k__mem_alloc(sizeof(struct k_alarm_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__alarm_callback_manager_del_callback;

    callback->manager = manager;
    callback->context = K__ALARM_CALLBACK_CONTEXT_ROOM;
    callback->state   = K__ALARM_CALLBACK_STATE_INACTIVE;

    /* [?] 应该使用当前时间，还是当前帧时间 */
    callback->timeout = k_time_get_step_timestamp() + delay_ms;

    k_list_insert_tail(&manager->pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_room_callback = fn_callback;
    callback->data = data;
    k_list_insert_tail(&room->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

struct k_callback *k__alarm_callback_manager_add_object_callback(struct k_alarm_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms) {

    struct k_alarm_callback *callback = k__mem_alloc(sizeof(struct k_alarm_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__alarm_callback_manager_del_callback;

    callback->manager = manager;
    callback->context = K__ALARM_CALLBACK_CONTEXT_OBJECT;
    callback->state   = K__ALARM_CALLBACK_STATE_INACTIVE;

    /* [?] 应该使用当前时间，还是当前帧时间 */
    callback->timeout = k_time_get_step_timestamp() + delay_ms;

    k_list_insert_tail(&manager->pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_object_callback = fn_callback;
    callback->object = object;
    k_list_insert_tail(&object->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

struct k_callback *k__alarm_callback_manager_add_component_callback(struct k_alarm_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms) {

    struct k_alarm_callback *callback = k__mem_alloc(sizeof(struct k_alarm_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__alarm_callback_manager_del_callback;

    callback->manager = manager;
    callback->context = K__ALARM_CALLBACK_CONTEXT_COMPONENT;
    callback->state   = K__ALARM_CALLBACK_STATE_INACTIVE;

    /* [?] 应该使用当前时间，还是当前帧时间 */
    callback->timeout = k_time_get_step_timestamp() + delay_ms;

    k_list_insert_tail(&manager->pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_component_callback = fn_callback;
    callback->component = component;
    k_list_insert_tail(&component->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

struct k_callback *k__alarm_callback_manager_add_component_manager_callback(struct k_alarm_callback_manager *manager, struct k_component_manager *component_manager, void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms) {

    struct k_alarm_callback *callback = k__mem_alloc(sizeof(struct k_alarm_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__alarm_callback_manager_del_callback;

    callback->manager = manager;
    callback->context = K__ALARM_CALLBACK_CONTEXT_COMPONENT_MANAGER;
    callback->state   = K__ALARM_CALLBACK_STATE_INACTIVE;

    /* [?] 应该使用当前时间，还是当前帧时间 */
    callback->timeout = k_time_get_step_timestamp() + delay_ms;

    k_list_insert_tail(&manager->pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_component_manager_callback = fn_callback;
    callback->data = data;
    k_list_insert_tail(&component_manager->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

/* endregion */

/* region [init] */

void k__alarm_callback_manager_init(struct k_alarm_callback_manager *manager) {
    k_list_init(&manager->callback_list);
    k_list_init(&manager->pending_list);
}

void k__alarm_callback_manager_deinit(struct k_alarm_callback_manager *manager) {

    k__alarm_callback_manager_flush(manager);

    struct k_alarm_callback *callback;
    struct k_list *list = &manager->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_alarm_callback, callback_list_node);

        k__mem_free(callback);
    }
}

/* endregion */

/* region [exec] */

void k__alarm_callback_manager_flush(struct k_alarm_callback_manager *manager) {

    struct k_alarm_callback *callback;
    struct k_list *list = &manager->pending_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_alarm_callback, pending_list_node);

        switch (callback->state) {
            case K__ALARM_CALLBACK_STATE_INACTIVE:
                k_list_remove(&callback->pending_list_node);
                k_list_node_loop(&callback->pending_list_node);

                /* TODO 改用优先队列后，修改这个代码块 */
                {
                    uint64_t timeout = callback->timeout;

                    struct k_alarm_callback *callback_in_list;
                    struct k_list *callback_list = &manager->callback_list;
                    struct k_list_node *iter_;
                    for (k_list_for_each(callback_list, iter_)) {
                        callback_in_list = container_of(iter_, struct k_alarm_callback, callback_list_node);

                        if (timeout <= callback_in_list->timeout)
                            break;
                    }
                    k_list_insert_before(iter_, &callback->callback_list_node);
                }
                /* TODO 改用优先队列后，修改这个代码块 */

                callback->state = K__ALARM_CALLBACK_STATE_ACTIVE;
                break;
            case K__ALARM_CALLBACK_STATE_EXECUTED:
            case K__ALARM_CALLBACK_STATE_DELETED:
                k_list_remove(&callback->callback_list_node);
                k_list_remove(&callback->pending_list_node);
                k__mem_free(callback);
                break;
            default:
                assert(0);
        }
    }
}

void k__alarm_callback_manager_exec(struct k_alarm_callback_manager *manager) {

    /* [?] 应该使用当前时间，还是当前帧时间 */
    const uint64_t current_ms = k_time_get_step_timestamp();

    struct k_list *callback_list = &manager->callback_list;
    struct k_alarm_callback *callback;
    struct k_list_node *iter;
    for (k_list_for_each(callback_list, iter)) {
        callback = container_of(iter, struct k_alarm_callback, callback_list_node);

        if (current_ms < callback->timeout)
            break;

        if (K__ALARM_CALLBACK_STATE_ACTIVE != callback->state)
            continue;

        int timeout_diff = (int)(current_ms - callback->timeout);

        k_list_insert_tail(&manager->pending_list, &callback->pending_list_node);
        callback->state = K__ALARM_CALLBACK_STATE_EXECUTED;

        switch (callback->context) {
            case K__ALARM_CALLBACK_CONTEXT_ROOM:
                callback->fn_room_callback(callback->data, timeout_diff);
                break;
            case K__ALARM_CALLBACK_CONTEXT_OBJECT:
                callback->fn_object_callback(callback->object, timeout_diff);
                break;
            case K__ALARM_CALLBACK_CONTEXT_COMPONENT:
                callback->fn_component_callback(callback->component, timeout_diff);
                break;
            case K__ALARM_CALLBACK_CONTEXT_COMPONENT_MANAGER:
                callback->fn_component_manager_callback(callback->data, timeout_diff);
                break;
            default:
                assert(0);
        }

        k__alarm_callback_manager_del_callback(&callback->base);
    }
}

/* endregion */

/* endregion */

/* region [add_callback] */

struct k_callback *k_room_add_alarm_callback(void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms) {
    struct k_room *room = k__room_current;
    return k__alarm_callback_manager_add_room_callback(&room->alarm_callback_manager, room, data, fn_callback, delay_ms);
}

struct k_callback *k_object_add_alarm_callback(struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms) {
    return k__alarm_callback_manager_add_object_callback(&object->room->alarm_callback_manager, object, fn_callback, delay_ms);
}

struct k_callback *k_component_add_alarm_callback(struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms) {
    return k__alarm_callback_manager_add_component_callback(&component->object->room->alarm_callback_manager, component, fn_callback, delay_ms);
}

struct k_callback *k_component_manager_add_alarm_callback(struct k_component_manager *manager, void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms) {
    return k__alarm_callback_manager_add_component_manager_callback(&manager->room->alarm_callback_manager, manager, data, fn_callback, delay_ms);
}

/* endregion */
