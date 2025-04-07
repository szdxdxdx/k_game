#include "k_game/core/k_room.h"
#include "./k_room.h"

void *k_room_get_data(void) {
    return k__current_room->data;
}

float k_room_get_width(void) {
    return k__current_room->room_w;
}

float k_room_get_height(void) {
    return k__current_room->room_h;
}
