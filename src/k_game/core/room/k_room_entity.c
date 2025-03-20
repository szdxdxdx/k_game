#include "./_internal.h"

#include "../game/_public.h"

int k_room_get_width(void) {
    return k__game.current_room->room_w;
}

int k_room_get_height(void) {
    return k__game.current_room->room_h;
}

void *k_room_get_data(void) {
    return k__game.current_room->data;
}
