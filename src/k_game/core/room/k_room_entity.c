#include "./_internal.h"

#include "../game/_public.h"

void *k_room_get_data(struct k_room *room) {
    return room->data;
}

float k_room_get_width(struct k_room *room) {
    return room->room_w;
}

float k_room_get_height(struct k_room *room) {
    return room->room_h;
}

float k_get_room_width(void) {
    return k__game.current_room->room_w;
}

float k_get_room_height(void) {
    return k__game.current_room->room_h;
}

void *k_get_room_data(void) {
    return k__game.current_room->data;
}
