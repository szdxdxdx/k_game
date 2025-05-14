#ifndef K__IMAGE_CREATE_H
#define K__IMAGE_CREATE_H

#include "SDL_render.h"

struct k_image;

struct k_image *k__image_create(SDL_Texture *texture);

void k__image_destroy(struct k_image *image);

#endif
