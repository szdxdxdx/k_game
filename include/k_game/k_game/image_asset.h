#ifndef K_GAME__IMAGE_LOAD_H
#define K_GAME__IMAGE_LOAD_H

#include "SDL.h"

#include "k_list.h"

struct k_image {

    /* TODO registry node */

    /* TODO ref_count && is_deleted */

    int w, h;

    SDL_Texture *texture;
};

#endif
