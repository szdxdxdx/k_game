#ifndef K_SDL_INTERNAL_H
#define K_SDL_INTERNAL_H

#include "k_game.h"

#include "./_shared.h"

/* region [keyboard] */

void k__SDL_refresh_keyboard_state(void);
void k__SDL_handle_event_key_down(SDL_KeyboardEvent *event);
void k__SDL_handle_event_key_up(SDL_KeyboardEvent *event);

/* endregion */

/* region [mouse] */

void k__SDL_refresh_mouse_state(void);
void k__SDL_handle_event_mouse_button_down(SDL_MouseButtonEvent *event);
void k__SDL_handle_event_mouse_button_up(SDL_MouseButtonEvent *event);
void k__SDL_handle_event_mouse_motion(SDL_MouseMotionEvent *event);

/* endregion */

#endif
