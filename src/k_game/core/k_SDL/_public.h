#ifndef K_GAME__K_SDL__PUBLIC_H
#define K_GAME__K_SDL__PUBLIC_H

#include "SDL_render.h"
#include "SDL_events.h"

/* region [SDL_init] */

struct k_game_config;

int k__SDL_init(const struct k_game_config *config);

void k__SDL_quit(void);

/* endregion */

/* region [game_window] */

struct k_game_window {

    SDL_Window *window;

    SDL_Renderer *renderer;
};

extern struct k_game_window k__window;

/* endregion */

/* region [for room game loop] */

void k__handle_SDL_event_with_frame_delay(uint64_t room_step_interval_ms);

/* endregion */

#endif
