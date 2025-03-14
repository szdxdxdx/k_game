#include "k_log.h"

#include "../k_SDL/k_SDL.h"

#include "./k_image.h"

struct k_image *k_image_scale(struct k_image *image, int scaled_w, int scaled_h) {

    SDL_Texture* scaled_texture = NULL;

    if (NULL == image || scaled_w <= 0 || scaled_h <= 0)
        goto err;

    SDL_Renderer *renderer = k__window.renderer;

    scaled_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, scaled_w, scaled_h);
    if (NULL == scaled_texture) {
        k_log_error("Failed to create SDL texture: %s", SDL_GetError());
        goto err;
    }

    SDL_SetTextureBlendMode(scaled_texture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(renderer, scaled_texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, image->texture, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);

    struct k_image *new_image = k__image_create(scaled_texture);
    if (NULL == new_image)
        goto err;

    return new_image;

err:
    if (NULL != scaled_texture)
        SDL_DestroyTexture(scaled_texture);

    k_log_error("Failed to scale image");
    return NULL;
}
