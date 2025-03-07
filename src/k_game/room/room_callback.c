#include "./room.h"

struct k_room_callback *k_room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data) {

    struct k_step_callback_manager *manager = &room->step_callback_manager;
    struct k_room_callback *callback = k__add_room_step_callback(manager, fn_callback, data);
    return callback;
}
