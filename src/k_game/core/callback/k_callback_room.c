#include "./k_callback.h"

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

    k__callback_defer_del(callback->base);
}

void k_room_del_all_callbacks(struct k_room *room) {

    struct k_room_callback *callback;
    struct k_list *list = &room->callback_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        callback = container_of(iter, struct k_room_callback, list_node);

        k__callback_defer_del(callback->base);
    }

    k_list_init(&room->callback_list);
}

void k__room_free_all_callbacks(struct k_room *room) {

    struct k_room_callback *callback;
    struct k_list *list = &room->callback_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        callback = container_of(iter, struct k_room_callback, list_node);

        k__callback_force_del(callback->base);
    }

    k_list_init(&room->callback_list);
}
