#include "k_game/alloc.h"
#include "k_game/component.h"
#include "k_game/room.h"
#include "../object/k_object_entity.h"
#include "./k_component_callback.h"

/* region [component_callback_list_add] */

static inline void component_callback_list_add(struct k_component *component, struct k_component_callback *callback) {
    struct k_list *callback_list = &component->callbacks;
    k_list_add_tail(callback_list, &callback->callback_list_node);
}

static inline void component_callback_list_del(struct k_component_callback *callback) {
    k_list_del(&callback->callback_list_node);
}

/* endregion */

/* region [del_callback] */

static inline void del_callback(struct k_component_callback *callback) {
    k_room_del_callback(callback->room_callback);
    component_callback_list_del(callback);
    k_free(callback);
}

void k_component_del_callback(struct k_component_callback *callback) {

    if (NULL != callback)
        del_callback(callback);
}

/* endregion */

/* region [component_callback_list_init] */

void k__component_init_callback_list(struct k_component *component) {
    k_list_init(&component->callbacks);
}

void k__component_cleanup_callback_list(struct k_component *component) {
    struct k_list *callback_list = &component->callbacks;

    struct k_component_callback *callback;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_list, iter, next)) {
        callback = container_of(iter, struct k_component_callback, callback_list_node);

        del_callback(callback);
    }
}

/* endregion */

/* region [add_alarm_callback] */

static void alarm_callback_wrapper(void *data, int timeout_diff) {
   struct k_component_callback *callback = data;
   callback->room_callback = NULL; /* <- 确保不重复删除房间回调 */

   callback->fn_alarm_callback(callback->component, timeout_diff);
   del_callback(callback);
}

struct k_component_callback *k_component_add_alarm_callback(struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms) {

    struct k_component_callback *component_callback = k_malloc(sizeof(struct k_component_callback));
    if (NULL == component_callback)
        return NULL;

    struct k_room_callback *room_callback = k_room_add_alarm_callback(component->object->room, alarm_callback_wrapper, component_callback, delay_ms);
    if (NULL == room_callback) {
        k_free(component_callback);
        return NULL;
    }

    component_callback->component = component;
    component_callback->room_callback = room_callback;
    component_callback->fn_alarm_callback = fn_callback;
    component_callback_list_add(component, component_callback);

    return component_callback;
}

static void step_callback_wrapper(void *data) {
    struct k_component_callback *callback = data;
    callback->fn_step_callback(callback->component);
}

/* endregion */

/* region [add_step_callback] */

struct k_component_callback *k_component_add_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {

    struct k_component_callback *component_callback = k_malloc(sizeof(struct k_component_callback));
    if (NULL == component_callback)
        return NULL;

    struct k_room_callback *room_callback = k_room_add_step_callback(component->object->room, step_callback_wrapper, component_callback);
    if (NULL == room_callback) {
        k_free(component_callback);
        return NULL;
    }

    component_callback->component = component;
    component_callback->room_callback = room_callback;
    component_callback->fn_step_callback = fn_callback;
    component_callback_list_add(component, component_callback);

    return component_callback;
}

/* endregion */

/* region [add_draw_callback] */

static void draw_callback_wrapper(void *data) {
    struct k_component_callback *callback = data;
    callback->fn_draw_callback(callback->component);
}

struct k_component_callback *k_component_add_draw_callback(struct k_component *component, void (*fn_callback)(struct k_component *component), int z_index) {

    struct k_component_callback *component_callback = k_malloc(sizeof(struct k_component_callback));
    if (NULL == component_callback)
        return NULL;

    struct k_room_callback *room_callback = k_room_add_draw_callback(component->object->room, draw_callback_wrapper, component_callback, z_index);
    if (NULL == room_callback) {
        k_free(component_callback);
        return NULL;
    }

    component_callback->component = component;
    component_callback->room_callback = room_callback;
    component_callback->fn_draw_callback = fn_callback;
    component_callback_list_add(component, component_callback);

    return component_callback;
}

/* endregion */
