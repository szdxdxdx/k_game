#include "./k_room.h"

struct k_room_callback *k_room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {

    struct k_step_callback_manager *manager = &room->step_callback_manager;
    struct k_room_callback *callback = k__callback_add_room_step(manager, fn_callback, data);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&room->callback_list, &callback->list_node);
    return callback;
}

struct k_room_callback *k_room_add_alarm_callback(struct k_room *room, void (*fn_callback)(void *data, int timeout_diff), void *data, int delay_ms) {

    struct k_alarm_callback_manager *manager = &room->alarm_callback_manager;
    struct k_room_callback *callback = k__callback_add_room_alarm(manager, fn_callback, data, delay_ms);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&room->callback_list, &callback->list_node);
    return callback;
}

struct k_room_callback *k_room_add_draw_callback(struct k_room *room, void (*fn_callback)(void *data), void *data, int z_index) {

    struct k_draw_callback_manager *manager = &room->draw_callback_manager;
    struct k_room_callback *callback = k__callback_add_room_draw(manager, fn_callback, data, z_index);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&room->callback_list, &callback->list_node);
    return callback;
}

void k_room_del_callback(struct k_room_callback *callback) {

    if (NULL == callback)
        return;

    k_list_del(&callback->list_node);
    k__callback_set_deleted(callback->base);
}
