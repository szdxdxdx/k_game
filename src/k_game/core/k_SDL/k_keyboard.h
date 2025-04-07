#ifndef K__KEYBOARD_H
#define K__KEYBOARD_H

#include "SDL_events.h"

void k__SDL_refresh_keyboard_state(void);

void k__SDL_handle_event_key_down(SDL_KeyboardEvent *event);

void k__SDL_handle_event_key_up(SDL_KeyboardEvent *event);

#endif
