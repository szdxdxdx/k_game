#include "./_internal.h"

#include "../room/_public.h"

struct k_game_context k__game = {
    .current_room = NULL,
    .quit_game = 0,
};

struct k_room *k_get_current_room(void) {
    return k__game.current_room;
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
