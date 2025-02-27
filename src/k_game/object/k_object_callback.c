#include "k_game/alloc.h"
#include "k_game/object.h"
#include "k_game/room.h"
#include "./k_object_entity.h"

/* region [object_callback_list_add] */

static inline void object_callback_list_add(struct k_object *object, struct k_object_callback *callback) {
    k_list_add_tail(&object->callbacks, &callback->iter_node);
}

static inline void object_callback_list_del(struct k_object_callback *callback) {
    k_list_del(&callback->iter_node);
}

/* endregion */

/* region [del_callback] */

static inline void del_object_callback(struct k_object_callback *callback) {
    k_room_del_callback(callback->room_callback);
    object_callback_list_del(callback);
    k_free(callback);
}

void k_object_del_callback(struct k_object_callback *callback) {

    if (NULL != callback)
        del_object_callback(callback);
}

/* endregion */

/* region [object_callbacks_list_init] */

void k__object_init_callbacks_list(struct k_object *object) {
    k_list_init(&object->callbacks);
}

void k__object_cleanup_callbacks_list(struct k_object *object) {

    struct k_object_callback *callback;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(&object->callbacks, iter, next)) {
        callback = container_of(iter, struct k_object_callback, iter_node);

        del_object_callback(callback);
    }
}

/* endregion */

/* region [add_alarm_callback] */

static void alarm_callback_wrapper(void *data, int timeout_diff) {
    struct k_object_callback *callback = data;
    callback->room_callback = NULL; /* <- [?] 确保不重复删除房间回调 */

    /* [?] alarm_callback 结点应何时删除
     * [?] alarm callback 不应该用 `k_object_del_callback()` 删除自身
     */

    callback->fn_alarm_callback(callback->object, timeout_diff);
    del_object_callback(callback);
}

struct k_object_callback *k_object_add_alarm_callback(struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms) {
    /* TODO assert(NULL != object) */

    struct k_object_callback *object_callback = k_malloc(sizeof(struct k_object_callback));
    if (NULL == object_callback)
        return NULL;

    struct k_room_callback *room_callback = k_room_add_alarm_callback(object->room, alarm_callback_wrapper, object_callback, delay_ms);
    if (NULL == room_callback) {
        k_free(object_callback);
        return NULL;
    }

    object_callback->fn_alarm_callback = fn_callback;
    object_callback->object = object;
    object_callback->room_callback = room_callback;
    object_callback_list_add(object, object_callback);

    return object_callback;
}

/* endregion */

/* region [add_step_callback] */

static void step_callback_wrapper(void *data) {
    struct k_object_callback *callback = data;
    callback->fn_step_callback(callback->object);
}

struct k_object_callback *k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    struct k_object_callback *object_callback = k_malloc(sizeof(struct k_object_callback));
    if (NULL == object_callback)
        return NULL;

    struct k_room_callback *room_callback = k_room_add_step_callback(object->room, step_callback_wrapper, object_callback);
    if (NULL == room_callback) {
        k_free(object_callback);
        return NULL;
    }

    object_callback->fn_step_callback = fn_callback;
    object_callback->object = object;
    object_callback->room_callback = room_callback;
    object_callback_list_add(object, object_callback);

    return object_callback;
}

/* endregion */

/* region [add_draw_callback] */

static void draw_callback_wrapper(void *data) {
    struct k_object_callback *callback = data;
    callback->fn_draw_callback(callback->object);
}

struct k_object_callback *k_object_add_draw_callback(struct k_object *object, void (*fn_callback)(struct k_object *object), int z_index) {

    struct k_object_callback *object_callback = k_malloc(sizeof(struct k_object_callback));
    if (NULL == object_callback)
        return NULL;

    struct k_room_callback *room_callback = k_room_add_draw_callback(object->room, draw_callback_wrapper, object_callback, z_index);
    if (NULL == room_callback) {
        k_free(object_callback);
        return NULL;
    }

    object_callback->fn_draw_callback = fn_callback;
    object_callback->object = object;
    object_callback->room_callback = room_callback;
    object_callback_list_add(object, object_callback);

    return object_callback;
}

/* endregion */
