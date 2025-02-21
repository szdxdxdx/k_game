#ifndef K_GAME__SDL_KEYBOARD_H
#define K_GAME__SDL_KEYBOARD_H

#include "SDL_keycode.h"

#include "k_game/keyboard.h"

void k__refresh_keyboard(void);

void k__set_key_down(SDL_Keycode SDL_key_code);

void k__set_key_up(SDL_Keycode SDL_key_code);

#endif
