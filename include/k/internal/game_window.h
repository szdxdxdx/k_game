#ifndef K_GAME_WINDOW_H
#define K_GAME_WINDOW_H

#include "SDL.h"

struct k_game_window {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

extern struct k_game_window * const k_window;

int k_create_window(void);

void k_destroy_window(void);

#endif
