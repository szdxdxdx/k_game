#include "./_internal.h"

#include "../game/_public.h"

float k_get_room_width(void) {
    return k__game.current_room->room_w;
}

float k_get_room_height(void) {
    return k__game.current_room->room_h;
}

void *k_get_room_data(void) {
    return k__game.current_room->data;
}
