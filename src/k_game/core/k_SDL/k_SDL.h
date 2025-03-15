#ifndef K_GAME__K_SDL_H
#define K_GAME__K_SDL_H

#include "SDL_render.h"
#include "SDL_events.h"

#include "k_game.h"

/* region [init] */

struct k_game_config;

int k__SDL_init(const struct k_game_config *config);

void k__SDL_quit(void);

/* endregion */

/* region [window] */

struct k_game_window {

    SDL_Window *window;

    SDL_Renderer *renderer;
};

extern struct k_game_window k__window;

/* endregion */

/* region [keyboard] */

void k__refresh_keyboard(void); /* <- TODO rename */

void k__handle_SDL_key_down_event(SDL_KeyboardEvent *event);

void k__handle_SDL_key_up_event(SDL_KeyboardEvent *event);

/* endregion */

/* region [SDL_event] */

void k__poll_SDL_events(void);

/* endregion */

#endif
