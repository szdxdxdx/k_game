#include "k_game/core/k_room.h"
#include "./k_room.h"

void *k_room_get_data(void) {
    return k__room_current->data;
}

void *k_room_get_data_of(struct k_room *room) {
    return room->data;
}

float k_room_get_w(void) {
    return k__room_current->room_w;
}

float k_room_get_h(void) {
    return k__room_current->room_h;
}
