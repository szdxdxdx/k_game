#include "./k_room.h"

int k_room_get_width(struct k_room *room) {
    return room->room_w;
}

int k_room_get_height(struct k_room *room) {
    return room->room_h;
}

void *k_room_get_data(struct k_room *room) {
    return room->data;
}
