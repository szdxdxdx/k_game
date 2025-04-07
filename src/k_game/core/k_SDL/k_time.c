#include "SDL_timer.h"

#include "k_game/core/k_time.h"
#include "./k_time.h"

struct k_time_context k__time;

float k_get_step_delta(void) {
    return k__time.step_delta;
}

int k_get_step_delta_ms(void) {
    return k__time.step_delta_ms;
}

uint64_t k_get_step_timestamp(void) {
    return k__time.step_timestamp;
}

uint64_t k_get_timestamp(void) {
    return SDL_GetTicks64();
}
