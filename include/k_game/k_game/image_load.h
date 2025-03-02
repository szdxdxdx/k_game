#ifndef K_GAME__IMAGE_LOAD_H
#define K_GAME__IMAGE_LOAD_H

#include "SDL.h"

#include "k_list.h"

struct k_image {

    /* TODO registry node */

    int w, h;

    SDL_Texture *texture;
};

#endif
