#ifndef K_GAME__IMAGE_LOAD_H
#define K_GAME__IMAGE_LOAD_H

#include "SDL.h"

#include "k_list.h"

struct k_image {

    struct k_list_node iter_node;

    int w, h;

    SDL_Texture *texture;
};

#endif
