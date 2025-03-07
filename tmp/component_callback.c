#include "k_game_alloc.h"

#include "../room/room_callback.h"
#include "../object/object.h"

#include "./component.h"

/* region [component_callback_list] */

void k__component_init_callback_list(struct k_component *component) {
    k_list_init(&component->callback_list);
}

void k__component_cleanup_callback_list(struct k_component *component) {

    struct k_component_callback *callback;
    struct k_list *callback_list = &component->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_list, iter, next)) {
        callback = container_of(iter, struct k_component_callback, component_callback_list_node);

        k_room_del_callback(callback->room_callback);
        k_free(callback);
    }

    k_list_init(&component->callback_list);
}

/* endregion */

/* region [component_del_callback] */

void k_component_del_callback(struct k_component_callback *callback) {

    if (NULL == callback)
        return;

    if (NULL == callback->room_callback) {
        /* TODO 更好的 `component_del_callback()` 方案
         *
         * 目前只有 alarm 回调需要判断 `room_callback` 是否为 `NULL`。
         * 因为 alarm 回调只执行一次，执行完会释放结点的内存，
         * 为了防止重复释放内存，这里用 `room_callback == NULL` 来做标记，
         * 使得在执行 alarm 回调时，使用 `component_del_callback()` 仍是安全的。
         * 但无法判断是否多次调用了 `component_del_callback()`。
         */
        return;
    }

    k_room_del_callback(callback->room_callback);
    k_list_del(&callback->component_callback_list_node);
    k_free(callback);
}

/* endregion */

/* region [component_add_alarm_callback] */

static void fn_alarm_callback_wrapper(void *data, int timeout_diff) {
    struct k_component_callback *callback = data;

    /* TODO 更好的 `component_del_callback()` 方案
     *
     * 因为 alarm 回调只执行一次，执行完会释放结点的内存，
     * 为了防止重复释放内存，这里用 `room_callback == NULL` 来做标记，
     * 使得在执行 alarm 回调时，使用 `component_del_callback()` 仍是安全的。
     * 但无法判断是否多次调用了 `component_del_callback()`。
     */
    callback->room_callback = NULL;

    callback->fn_alarm_callback(callback->component, timeout_diff);

    k_list_del(&callback->component_callback_list_node);
    k_free(callback);
}

struct k_component_callback *k_component_add_alarm_callback(struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms) {

    struct k_component_callback *component_callback = k_malloc(sizeof(struct k_component_callback));
    if (NULL == component_callback)
        return NULL;

    struct k_room_callback *room_callback = k__room_add_alarm_callback(component->object->room, fn_alarm_callback_wrapper, component_callback, delay_ms);
    if (NULL == room_callback) {
        k_free(component_callback);
        return NULL;
    }

    component_callback->component = component;
    component_callback->room_callback = room_callback;
    component_callback->fn_alarm_callback = fn_callback;
    k_list_add_tail(&component->callback_list, &component_callback->component_callback_list_node);

    return component_callback;
}

/* endregion */

/* region [component_add_step_callback] */

static void fn_step_callback_wrapper(void *data) {
    struct k_component_callback *callback = data;
    callback->fn_step_callback(callback->component);
}

struct k_component_callback *k_component_add_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {

    struct k_component_callback *component_callback = k_malloc(sizeof(struct k_component_callback));
    if (NULL == component_callback)
        return NULL;

    struct k_room_callback *room_callback = k__room_add_step_callback(component->object->room, fn_step_callback_wrapper, component_callback);
    if (NULL == room_callback) {
        k_free(component_callback);
        return NULL;
    }

    component_callback->component = component;
    component_callback->room_callback = room_callback;
    component_callback->fn_step_callback = fn_callback;
    k_list_add_tail(&component->callback_list, &component_callback->component_callback_list_node);

    return component_callback;
}

/* endregion */

/* region [component_add_draw_callback] */

static void fn_draw_callback_wrapper(void *data) {
    struct k_component_callback *callback = data;
    callback->fn_draw_callback(callback->component);
}

struct k_component_callback *k_component_add_draw_callback(struct k_component *component, void (*fn_callback)(struct k_component *component), int z_index) {

    struct k_component_callback *component_callback = k_malloc(sizeof(struct k_component_callback));
    if (NULL == component_callback)
        return NULL;

    struct k_room_callback *room_callback = k__room_add_draw_callback(component->object->room, fn_draw_callback_wrapper, component_callback, z_index);
    if (NULL == room_callback) {
        k_free(component_callback);
        return NULL;
    }

    component_callback->component = component;
    component_callback->room_callback = room_callback;
    component_callback->fn_draw_callback = fn_callback;
    k_list_add_tail(&component->callback_list, &component_callback->component_callback_list_node);

    return component_callback;
}

/* endregion */
