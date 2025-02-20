#ifndef K_GAME__SDL_WINDOW_H
#define K_GAME__SDL_WINDOW_H

#include "SDL_render.h"

struct k_game_window {

    SDL_Window *window;

    SDL_Renderer *renderer;
};

extern struct k_game_window k__window;

#endif
