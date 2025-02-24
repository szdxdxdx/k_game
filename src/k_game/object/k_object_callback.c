
#include "k_game/alloc.h"
#include "k_game/object.h"
#include "k_game/room.h"
#include "./k_object_entity.h"

/* region [object_callbacks_list] */

void k__object_init_callbacks_list(struct k_object *object) {
    struct k_object_callbacks_list *callbacks_list = &object->callbacks_list;

    k_list_init(&callbacks_list->list);
}

void k__object_cleanup_callbacks_list(struct k_object *object) {
    struct k_object_callbacks_list *callbacks_list = &object->callbacks_list;

    /* TODO foreach callbacks_list : del callback */
}

static inline void object_callbacks_list_add(struct k_object_callbacks_list *list, struct k_object_callback *callback) {
    k_list_add_tail(&list->list, &callback->iter_node);
}

static inline void object_callbacks_list_del(struct k_object_callback *callback) {
    k_list_del(&callback->iter_node);
}

/* endregion */

/* region [room_callback_wrapper] */

static void room_alarm_callback_wrapper(void *data, int timeout_diff) {
    struct k_object_callback *callback = data;

    /* TODO alarm_callback 结点应何时删除？
     *
     * alarm callback 不应该用 `k_object_del_callback()` 删除自身？
     */

    callback->fn_alarm_callback(callback->object, timeout_diff);

    object_callbacks_list_del(callback);
    k_free(callback);
}

static void room_step_callback_wrapper(void *data) {
    struct k_object_callback *callback = data;
    callback->fn_step_callback(callback->object);
}

static void room_draw_callback_wrapper(void *data) {
    struct k_object_callback *callback = data;
    callback->fn_draw_callback(callback->object);
}

/* endregion */

void k_object_del_callback(struct k_object_callback *callback) {

    if (NULL != callback) {
        k_room_del_callback(callback->room_callback);
        object_callbacks_list_del(callback);
        k_free(callback);
    }
}

struct k_object_callback *k_object_add_alarm_callback(struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms) {

    struct k_object_callback *object_callback = k_malloc(sizeof(struct k_object_callback));
    if (NULL == object_callback)
        return NULL;

    struct k_room_callback *room_callback = k_room_add_alarm_callback(object->room, room_alarm_callback_wrapper, object_callback, delay_ms);
    if (NULL == room_callback) {
        k_free(object_callback);
        return NULL;
    }

    object_callback->fn_alarm_callback = fn_callback;
    object_callback->object = object;
    object_callback->room_callback = room_callback;
    object_callbacks_list_add(&object->callbacks_list, object_callback);

    return object_callback;
}

struct k_object_callback *k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    struct k_object_callback *object_callback = k_malloc(sizeof(struct k_object_callback));
    if (NULL == object_callback)
        return NULL;

    struct k_room_callback *room_callback = k_room_add_step_callback(object->room, room_step_callback_wrapper, object_callback);
    if (NULL == room_callback) {
        k_free(object_callback);
        return NULL;
    }

    object_callback->fn_step_callback = fn_callback;
    object_callback->object = object;
    object_callback->room_callback = room_callback;
    object_callbacks_list_add(&object->callbacks_list, object_callback);

    return object_callback;
}

struct k_object_callback *k_object_add_draw_callback(struct k_object *object, void (*fn_callback)(struct k_object *object), int z_index) {

    struct k_object_callback *object_callback = k_malloc(sizeof(struct k_object_callback));
    if (NULL == object_callback)
        return NULL;

    struct k_room_callback *room_callback = k_room_add_draw_callback(object->room, room_draw_callback_wrapper, object_callback, z_index);
    if (NULL == room_callback) {
        k_free(object_callback);
        return NULL;
    }

    object_callback->fn_draw_callback = fn_callback;
    object_callback->object = object;
    object_callback->room_callback = room_callback;
    object_callbacks_list_add(&object->callbacks_list, object_callback);

    return object_callback;
}
