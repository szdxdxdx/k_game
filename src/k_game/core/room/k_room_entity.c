#include "./_internal.h"

#include "../game/_public.h"

void *k_room_get_data(struct k_room *room) {

    if (K_CURRENT_ROOM == room) {
        room = k__game.current_room;
    }

    return room->data;
}

float k_room_get_width(struct k_room *room) {

    if (K_CURRENT_ROOM == room) {
        room = k__game.current_room;
    }

    return room->room_w;
}

float k_room_get_height(struct k_room *room) {

    if (K_CURRENT_ROOM == room) {
        room = k__game.current_room;
    }

    return room->room_h;
}
