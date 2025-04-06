#ifndef K_SDL_SHARED_H
#define K_SDL_SHARED_H

/* region [SDL_init] */

struct k_game_config;

int k__SDL_init(const struct k_game_config *config);

void k__SDL_quit(void);

/* endregion */

/* region [for room game loop] */

void k__SDL_handle_event_with_frame_delay(uint64_t room_step_interval_ms);

/* endregion */

#endif
