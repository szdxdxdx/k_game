#ifndef K_GAME__SDL_KEYBOARD_H
#define K_GAME__SDL_KEYBOARD_H

#include "SDL_keycode.h"

#include "k_game/keyboard.h"

void k__refresh_keyboard(void);

enum k_keyboard_key k__SDL_key_map(SDL_Keycode SDL_key_code);

#endif
