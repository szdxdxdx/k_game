#ifndef K_GAME__K_SDL__INTERNAL_H
#define K_GAME__K_SDL__INTERNAL_H

#include "k_game.h"

#include "./_public.h"

void k__handle_SDL_key_down_event(SDL_KeyboardEvent *event);

void k__handle_SDL_key_up_event(SDL_KeyboardEvent *event);

void k__handle_SDL_mouse_motion_event(SDL_MouseMotionEvent *event);

#endif
