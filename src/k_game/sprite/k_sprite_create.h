#ifndef K_GAME__SPRITE_CREATE_H
#define K_GAME__SPRITE_CREATE_H

#include "SDL.h"

struct k_sprite_frame {

    SDL_Texture *texture;

    struct k_int_point offset;

    int delay;
};

struct k_sprite {

    /* TODO registry node */

    const char *sprite_name;

    int sprite_w, sprite_h;

    int origin_x, origin_y;

    struct k_sprite_frame *frames;

    int frames_num;
};

#endif
