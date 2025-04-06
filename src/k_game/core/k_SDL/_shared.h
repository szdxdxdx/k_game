#ifndef K_SDL_SHARED_H
#define K_SDL_SHARED_H

#include "SDL_render.h"
#include "SDL_events.h"

/* region [SDL_init] */

struct k_game_config;

int k__SDL_init(const struct k_game_config *config);

void k__SDL_quit(void);

/* endregion */

/* region [SDL_context] */

struct k_SDL_context {

    SDL_Window *window;
    int window_w;
    int window_h;

    SDL_Renderer *renderer;

    SDL_Texture *canvas;
    int canvas_w;
    int canvas_h;
};

extern struct k_SDL_context k__SDL;

/* endregion */

/* region [for room game loop] */

void k__SDL_handle_event_with_frame_delay(uint64_t room_step_interval_ms);

/* endregion */

#endif
