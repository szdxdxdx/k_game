#ifndef K_GAME__SDL_KEYBOARD_H
#define K_GAME__SDL_KEYBOARD_H

#include "SDL_events.h"

#include "k_game/keyboard.h"

void k__refresh_keyboard(void);

void k__handle_SDL_key_down_event(SDL_KeyboardEvent *event);

void k__handle_SDL_key_up_event(SDL_KeyboardEvent *event);

#endif
