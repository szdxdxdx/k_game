#include "../room/k_room.h"

struct k_room_callback *k_room_add_step_begin_callback(struct k_room *room, void *data, void (*fn_callback)(void *data)) {
    return k__callback_add_room_step(&room->step_begin_callback_manager, room, data, fn_callback);
}

struct k_room_callback *k_room_add_alarm_callback(struct k_room *room, void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms) {
    return k__callback_add_room_alarm(&room->alarm_callback_manager, room, data, fn_callback, delay_ms);
}

struct k_room_callback *k_room_add_step_callback(struct k_room *room, void *data, void (*fn_callback)(void *data)) {
    return k__callback_add_room_step(&room->step_callback_manager, room, data, fn_callback);
}

struct k_room_callback *k_room_add_draw_callback(struct k_room *room, void *data, void (*fn_callback)(void *data), int z_index) {
    return k__callback_add_room_draw(&room->draw_callback_manager, room, data, fn_callback, z_index);
}

struct k_room_callback *k_room_add_step_end_callback(struct k_room *room, void *data, void (*fn_callback)(void *data)) {
    return k__callback_add_room_step(&room->step_end_callback_manager, room, data, fn_callback);
}

void k_room_del_callback(struct k_room_callback *callback) {

    if (NULL == callback)
        return;

    if (k_list_node_is_loop(&callback->list_node))
        return;

    k_list_del(&callback->list_node);
    k_list_node_loop(&callback->list_node);

    k__callback_set_deleted(callback->base);
}

void k_room_del_all_callback(struct k_room_callback *callback) {

    /* [?] 应该删除房间的所有回调吗？
     * step、alarm 这些可以删，但是 enter 和 leave 呢？
     * fn_init 和 fn_cleanup 也是回调，这些又该怎么论？
     */
}
