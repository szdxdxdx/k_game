#include "./_internal.h"

#include "../game/_shared.h"

struct k_room *K_CURRENT_ROOM = (void *)0x7;

void *k_room_get_data(struct k_room *room) {
    return room->data;
}

float k_room_get_width(struct k_room *room) {
    return room->room_w;
}

float k_room_get_height(struct k_room *room) {
    return room->room_h;
}
