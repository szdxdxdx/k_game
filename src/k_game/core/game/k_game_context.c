#include "SDL_timer.h"

#include "./k_game_context.h"

struct k_game_context k__game;

float k_get_step_delta(void) {
    return k__game.step_delta;
}

int k_get_step_delta_ms(void) {
    return k__game.step_delta_ms;
}

uint64_t k_get_step_timestamp(void) {
    return k__game.step_timestamp;
}

uint64_t k_get_timestamp(void) {
    return SDL_GetTicks64();
}
