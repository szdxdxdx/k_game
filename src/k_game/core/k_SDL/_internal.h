#ifndef K_SDL_INTERNAL_H
#define K_SDL_INTERNAL_H

#include "k_game.h"

#include "./_public.h"

/* region [keyboard] */

void k__refresh_key_state(void);
void k__handle_SDL_event_key_down(SDL_KeyboardEvent *event);
void k__handle_SDL_event_key_up(SDL_KeyboardEvent *event);

/* endregion */

/* region [mouse] */

void k__refresh_mouse_state(void);
void k__handle_SDL_event_mouse_button_down(SDL_MouseButtonEvent *event);
void k__handle_SDL_event_mouse_button_up(SDL_MouseButtonEvent *event);
void k__handle_SDL_event_mouse_motion(SDL_MouseMotionEvent *event);

/* endregion */

#endif
