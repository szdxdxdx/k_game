#include "./room.h"

struct k_room_callback *k_room_add_enter_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    /* TODO assert( NULL != room ... ) */
    return k__room_add_enter_callback(room, fn_callback, data);
}

struct k_room_callback *k_room_add_leave_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    /* TODO assert( NULL != room ... ) */
    return k__room_add_leave_callback(room, fn_callback, data);
}

struct k_room_callback *k_room_add_step_begin_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    /* TODO assert( NULL != room ... ) */
    return k__room_add_step_begin_callback(room, fn_callback, data);
}

struct k_room_callback *k_room_add_alarm_callback(struct k_room *room, void (*fn_callback)(void *data, int timeout_diff), void *data, int delay_ms) {
    /* TODO assert( NULL != room ... ) */
    return k__room_add_alarm_callback(room, fn_callback, data, delay_ms);
}

struct k_room_callback *k_room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    /* TODO assert( NULL != room ... ) */
    return k__room_add_step_callback(room, fn_callback, data);
}

struct k_room_callback *k_room_add_draw_callback(struct k_room *room, void (*fn_callback)(void *data), void *data, int z_index) {
    /* TODO assert( NULL != room ... ) */
    return k__room_add_draw_callback(room, fn_callback, data, z_index);
}

struct k_room_callback *k_room_add_step_end_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {
    /* TODO assert( NULL != room ... ) */
    return k__room_add_step_end_callback(room, fn_callback, data);
}

void k_room_del_callback(struct k_room_callback *callback) {

    if (NULL == callback)
        return;

    /*
     * TODO assert( ! callback->is_deleted )
     *
     * 虽然 `k_room_del_callback()` 不会立即释放结点的内存，
     * 但也不能重复调用，这就跟不能重复调用 `free()` 一样。
     */

    callback->is_deleted = 1;
}
