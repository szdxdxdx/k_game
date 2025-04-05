#ifndef K_SDL_PUBLIC_H
#define K_SDL_PUBLIC_H

#include "SDL_render.h"
#include "SDL_events.h"

/* region [SDL_init] */

struct k_game_config;

int k__init_SDL(const struct k_game_config *config);

void k__quit_SDL(void);

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

void k__handle_SDL_event_with_frame_delay(uint64_t room_step_interval_ms);

/* endregion */

#endif
