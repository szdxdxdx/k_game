#include "k_game/room.h"
#include "./k_game_context.h"
#include "../room/k_room_context.h"

struct k_game_context k__game = {
    .current_room = NULL,
    .quit_game = 0,
};

struct k_room *k_get_current_room(void) {
    return k__game.current_room;
}

void *k_get_current_room_data(void) {
    return k__game.current_room->data;
}
