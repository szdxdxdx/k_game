#include "./k_room.h"

#include "../game/k_game_context.h"

void *k_room_get_data(void) {
    return k__game.current_room->data;
}

float k_room_get_width(void) {
    return k__game.current_room->room_w;
}

float k_room_get_height(void) {
    return k__game.current_room->room_h;
}
