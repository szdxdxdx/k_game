#include "k_game/room.h"
#include "k_game/time.h"
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

float k_get_step_delta(void) {
    return k__game.step_delta;
}

int k_get_step_delta_ms(void) {
    return k__game.step_delta_ms;
}

uint64_t k_get_step_timestamp(void) {
    return k__game.step_timestamp;
}
