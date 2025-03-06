#include "k_game_alloc.h"
#include "k_game_room.h"
#include "../room/room_callback.h"
#include "k_game_object.h"
#include "./object_entity.h"
#include "./object_callback.h"

/* region [object_callback_list] */

void k__object_init_callback_list(struct k_object *object) {
    k_list_init(&object->callback_list);
}

void k__object_cleanup_callback_list(struct k_object *object) {

    struct k_object_callback *callback;
    struct k_list *callback_list = &object->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_list, iter, next)) {
        callback = container_of(iter, struct k_object_callback, object_callback_list_node);

        k_room_del_callback(callback->room_callback);
        k_free(callback);
    }

    k_list_init(&object->callback_list);
}

/* endregion */

/* region [object_del_callback] */

void k_object_del_callback(struct k_object_callback *callback) {

    if (NULL == callback)
        return;

    if (NULL == callback->room_callback) {
        /* TODO 更好的 `object_del_callback()` 方案
         *
         * 目前只有 alarm 回调需要判断 `room_callback` 是否为 `NULL`。
         * 因为 alarm 回调只执行一次，执行完会释放结点的内存，
         * 为了防止重复释放内存，这里用 `room_callback == NULL` 来做标记，
         * 使得在执行 alarm 回调时，使用 `object_del_callback()` 仍是安全的。
         */
        return;
    }

    k_room_del_callback(callback->room_callback);
    k_list_del(&callback->object_callback_list_node);
    k_free(callback);
}

void k_object_del_all_callbacks(struct k_object *object) {
    /* assert( NULL != object) */
    k__object_cleanup_callback_list(object);
}

/* endregion */

/* region [object_add_step_begin_callback] */

static void step_begin_callback_wrapper(void *data) {
    struct k_object_callback *callback = data;
    callback->fn_step_begin_callback(callback->object);
}

struct k_object_callback *k_object_add_step_begin_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    struct k_object_callback *object_callback = k_malloc(sizeof(struct k_object_callback));
    if (NULL == object_callback)
        return NULL;

    struct k_room_callback *room_callback = k__room_add_step_begin_callback(object->room, step_begin_callback_wrapper, object_callback);
    if (NULL == room_callback) {
        k_free(object_callback);
        return NULL;
    }

    object_callback->fn_step_begin_callback = fn_callback;
    object_callback->object = object;
    object_callback->room_callback = room_callback;
    k_list_add_tail(&object->callback_list, &object_callback->object_callback_list_node);

    return object_callback;
}

/* endregion */

/* region [object_add_alarm_callback] */

static void alarm_callback_wrapper(void *data, int timeout_diff) {
    struct k_object_callback *callback = data;

    /* TODO 更好的 `component_del_callback()` 方案
     *
     * 因为 alarm 回调只执行一次，执行完会释放结点的内存，
     * 为了防止重复释放内存，这里用 `room_callback == NULL` 来做标记，
     * 使得在执行 alarm 回调时，使用 `component_del_callback()` 仍是安全的。
     */
    callback->room_callback = NULL;

    callback->fn_alarm_callback(callback->object, timeout_diff);

    k_list_del(&callback->object_callback_list_node);
    k_free(callback);
}

struct k_object_callback *k_object_add_alarm_callback(struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms) {
    /* TODO assert(NULL != object) */

    struct k_object_callback *object_callback = k_malloc(sizeof(struct k_object_callback));
    if (NULL == object_callback)
        return NULL;

    struct k_room_callback *room_callback = k__room_add_alarm_callback(object->room, alarm_callback_wrapper, object_callback, delay_ms);
    if (NULL == room_callback) {
        k_free(object_callback);
        return NULL;
    }

    object_callback->fn_alarm_callback = fn_callback;
    object_callback->object = object;
    object_callback->room_callback = room_callback;
    k_list_add_tail(&object->callback_list, &object_callback->object_callback_list_node);

    return object_callback;
}

/* endregion */

/* region [object_add_step_callback] */

static void step_callback_wrapper(void *data) {
    struct k_object_callback *callback = data;
    callback->fn_step_callback(callback->object);
}

struct k_object_callback *k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    struct k_object_callback *object_callback = k_malloc(sizeof(struct k_object_callback));
    if (NULL == object_callback)
        return NULL;

    struct k_room_callback *room_callback = k__room_add_step_callback(object->room, step_callback_wrapper, object_callback);
    if (NULL == room_callback) {
        k_free(object_callback);
        return NULL;
    }

    object_callback->fn_step_callback = fn_callback;
    object_callback->object = object;
    object_callback->room_callback = room_callback;
    k_list_add_tail(&object->callback_list, &object_callback->object_callback_list_node);

    return object_callback;
}

/* endregion */

/* region [object_add_draw_callback] */

static void draw_callback_wrapper(void *data) {
    struct k_object_callback *callback = data;
    callback->fn_draw_callback(callback->object);
}

struct k_object_callback *k_object_add_draw_callback(struct k_object *object, void (*fn_callback)(struct k_object *object), int z_index) {

    struct k_object_callback *object_callback = k_malloc(sizeof(struct k_object_callback));
    if (NULL == object_callback)
        return NULL;

    struct k_room_callback *room_callback = k__room_add_draw_callback(object->room, draw_callback_wrapper, object_callback, z_index);
    if (NULL == room_callback) {
        k_free(object_callback);
        return NULL;
    }

    object_callback->fn_draw_callback = fn_callback;
    object_callback->object = object;
    object_callback->room_callback = room_callback;
    k_list_add_tail(&object->callback_list, &object_callback->object_callback_list_node);

    return object_callback;
}

/* endregion */

/* region [object_add_step_end_callback] */

static void step_end_callback_wrapper(void *data) {
    struct k_object_callback *callback = data;
    callback->fn_step_end_callback(callback->object);
}

struct k_object_callback *k_object_add_step_end_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {
    struct k_object_callback *object_callback = k_malloc(sizeof(struct k_object_callback));
    if (NULL == object_callback)
        return NULL;

    struct k_room_callback *room_callback = k__room_add_step_end_callback(object->room, step_end_callback_wrapper, object_callback);
    if (NULL == room_callback) {
        k_free(object_callback);
        return NULL;
    }

    object_callback->fn_step_end_callback = fn_callback;
    object_callback->object = object;
    object_callback->room_callback = room_callback;
    k_list_add_tail(&object->callback_list, &object_callback->object_callback_list_node);

    return object_callback;
}

/* endregion */
