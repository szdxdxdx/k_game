#include "./_internal.h"

struct k_room *K_CURRENT_ROOM = NULL;

void *k_room_get_data(struct k_room *room) {
    return room->data;
}

float k_room_get_width(struct k_room *room) {
    return room->room_w;
}

float k_room_get_height(struct k_room *room) {
    return room->room_h;
}
