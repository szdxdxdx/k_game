#ifndef K_GAME_TIME_H
#define K_GAME_TIME_H

#include <stdint.h>

uint64_t k_get_current_step_time(void);

int k_get_step_delta_ms(void);

float k_get_step_delta(void);

#endif
