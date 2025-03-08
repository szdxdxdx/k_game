#include "k_game_alloc.h"

#include "./callback.h"
#include "../game/game.h"

/* region [callback_def] */

struct k_alarm_callback {

    struct k_list_node list_node;

    struct k_callback base;

    uint64_t timeout;
};

struct k_room_alarm_callback {

    struct k_alarm_callback alarm_callback; /* inherit */

    struct k_room_callback room_callback;

    void (*fn_callback)(void *data,  int timeout_diff);

    void *data;
};

struct k_object_alarm_callback {

    struct k_alarm_callback alarm_callback; /* inherit */

    struct k_object_callback object_callback;

    void (*fn_callback)(struct k_object *object, int timeout_diff);

    struct k_object *object;
};

struct k_component_alarm_callback {

    struct k_alarm_callback alarm_callback; /* inherit */

    struct k_component_callback component_callback;

    void (*fn_callback)(struct k_component *component, int timeout_diff);

    struct k_component *component;
};

/* endregion */

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
        alarm_callback = container_of(iter, struct k_alarm_callback, list_node);
        k_free(alarm_callback);
    }
}

void k__callback_flush_alarm(struct k_alarm_callback_manager *manager) {

    struct k_alarm_callback *alarm_callback;
    struct k_list *list = &manager->pending_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        alarm_callback = container_of(iter, struct k_alarm_callback, list_node);

        k_list_del(&alarm_callback->list_node);

        /* TODO 改用优先队列后，修改这个代码块 -> */ {
            uint64_t timeout = alarm_callback->timeout;

            struct k_alarm_callback *callback_in_list;
            struct k_list *callback_list = &manager->callback_list;
            struct k_list_node *iter_;
            for (k_list_for_each(callback_list, iter_)) {
                callback_in_list = container_of(iter_, struct k_alarm_callback, list_node);

                if (timeout <= callback_in_list->timeout)
                    break;
            }

            k_list_add(iter_->prev, &alarm_callback->list_node);
        }
    }
}

void k__callback_exec_alarm(struct k_alarm_callback_manager *manager) {

    /* [?] 应该使用当前时间，还是当前帧时间 */
    const uint64_t current_ms = k__game.step_timestamp;

    struct k_list *callback_list = &manager->callback_list;
    struct k_alarm_callback *alarm_callback;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_list, iter, next)) {
        alarm_callback = container_of(iter, struct k_alarm_callback, list_node);

        if (alarm_callback->base.deleted)
            goto del_callback;

        if (current_ms < alarm_callback->timeout)
            break;

        int timeout_diff = (int)(current_ms - alarm_callback->timeout);

        switch (alarm_callback->base.context) {
            case K_ROOM_CALLBACK: {
                struct k_room_alarm_callback *callback = (struct k_room_alarm_callback *)alarm_callback;

                k_list_del(&callback->room_callback.list_node); /* 摘链并自环，原因同 case K_OBJECT_CALLBACK */
                k_list_link_self(&callback->room_callback.list_node);

                callback->fn_callback(callback->data, timeout_diff);
                break;
            }
            case K_OBJECT_CALLBACK: {
                struct k_object_alarm_callback *callback = (struct k_object_alarm_callback *)alarm_callback;

                /* alarm 回调仅执行一次，执行完后由 manager 删除回调结点并释放内存。
                 *
                 * 由于删除操作由 manager 代劳，而非 object 直接调用 `k_object_del_callback()`，
                 * 因此 manager 需模拟 `k_object_del_callback()` 的部分逻辑。
                 *
                 * 先将回调结点移出 object 的回调链表。
                 */
                k_list_del(&callback->object_callback.list_node);

                /* 若 object 在回调中执行 `k_object_del_callback()`，
                 * `k_object_del_callback()` 会再执行一次结点摘链操作。
                 *
                 * 此处将结点自环。对自环结点重复执行摘链操作是安全的。
                 */
                k_list_link_self(&callback->object_callback.list_node);

                callback->fn_callback(callback->object, timeout_diff);
                break;
            }
            case K_COMPONENT_CALLBACK: {
                struct k_component_alarm_callback *callback = (struct k_component_alarm_callback *)alarm_callback;

                k_list_del(&callback->component_callback.list_node); /* 摘链并自环，原因同 case K_OBJECT_CALLBACK */
                k_list_link_self(&callback->component_callback.list_node);

                callback->fn_callback(callback->component, timeout_diff);
                break;
            }
        }

    del_callback:
        k_list_del(&alarm_callback->list_node);
        k_free(alarm_callback);
    }
}

struct k_room_callback *k__callback_add_room_alarm(struct k_alarm_callback_manager *manager, void (*fn_callback)(void *data, int timeout_diff), void *data, int delay_ms) {

    struct k_room_alarm_callback *callback = k_malloc(sizeof(struct k_room_alarm_callback));
    if (NULL == callback)
        return NULL;

    /* [?] 应该使用当前时间，还是当前帧时间 */
    uint64_t timeout = k__game.step_timestamp + delay_ms;

    callback->alarm_callback.base.event   = K_ALARM_CALLBACK;
    callback->alarm_callback.base.context = K_ROOM_CALLBACK;
    callback->alarm_callback.base.deleted = 0;
    callback->alarm_callback.timeout      = timeout;
    callback->room_callback.base          = &callback->alarm_callback.base;
    callback->fn_callback                 = fn_callback;
    callback->data                        = data;
    k_list_add_tail(&manager->pending_list, &callback->alarm_callback.list_node);

    return &callback->room_callback;
}

struct k_object_callback *k__callback_add_object_alarm(struct k_alarm_callback_manager *manager, void (*fn_callback)(struct k_object *object, int timeout_diff), struct k_object *object, int delay_ms) {

    struct k_object_alarm_callback *callback = k_malloc(sizeof(struct k_object_alarm_callback));
    if (NULL == callback)
        return NULL;

    /* [?] 应该使用当前时间，还是当前帧时间 */
    uint64_t timeout = k__game.step_timestamp + delay_ms;

    callback->alarm_callback.base.event   = K_ALARM_CALLBACK;
    callback->alarm_callback.base.context = K_OBJECT_CALLBACK;
    callback->alarm_callback.base.deleted = 0;
    callback->alarm_callback.timeout      = timeout;
    callback->object_callback.base        = &callback->alarm_callback.base;
    callback->fn_callback                 = fn_callback;
    callback->object                      = object;
    k_list_add_tail(&manager->pending_list, &callback->alarm_callback.list_node);

    return &callback->object_callback;
}

struct k_component_callback *k__callback_add_component_alarm(struct k_alarm_callback_manager *manager, void (*fn_callback)(struct k_component *component, int timeout_diff), struct k_component *component, int delay_ms) {

    struct k_component_alarm_callback *callback = k_malloc(sizeof(struct k_component_alarm_callback));
    if (NULL == callback)
        return NULL;

    /* [?] 应该使用当前时间，还是当前帧时间 */
    uint64_t timeout = k__game.step_timestamp + delay_ms;

    callback->alarm_callback.base.event   = K_ALARM_CALLBACK;
    callback->alarm_callback.base.context = K_COMPONENT_CALLBACK;
    callback->alarm_callback.base.deleted = 0;
    callback->alarm_callback.timeout      = timeout;
    callback->component_callback.base     = &callback->alarm_callback.base;
    callback->fn_callback                 = fn_callback;
    callback->component                   = component;
    k_list_add_tail(&manager->pending_list, &callback->alarm_callback.list_node);

    return &callback->component_callback;
}
