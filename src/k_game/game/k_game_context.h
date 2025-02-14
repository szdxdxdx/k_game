#ifndef K_GAME__GAME_CONTEXT_H
#define K_GAME__GAME_CONTEXT_H

#include "SDL.h"

struct k_game_context {

    SDL_Window *window;

    SDL_Renderer *renderer;

    unsigned int quit_game;

    float delta_time;
};

extern struct k_game_context k_game;

#endif
