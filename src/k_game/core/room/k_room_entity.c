#include "./_internal.h"

struct k_room *K_CURRENT_ROOM = NULL;

void *k_room_get_data(void) {
    return K_CURRENT_ROOM->data;
}

float k_room_get_width(void) {
    return K_CURRENT_ROOM->room_w;
}

float k_room_get_height(void) {
    return K_CURRENT_ROOM->room_h;
}
