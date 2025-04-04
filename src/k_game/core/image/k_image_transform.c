#include "k_log.h"

#include "./_internal.h"

struct k_image *k_image_scale(struct k_image *image, int scaled_w, int scaled_h) {

    SDL_Texture* scaled_texture = NULL;

    if (NULL == image) {
        k_log_error("Failed to scale image, image is NULL");
        return NULL;
    } else if (scaled_w <= 0 || scaled_h <= 0) {
        k_log_error("Failed to scale image, scaled width and height must be greater than 0");
        goto err;
    }

    SDL_Renderer *renderer = k__SDL.renderer;

    scaled_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, scaled_w, scaled_h);
    if (NULL == scaled_texture) {
        k_log_error("Failed to scale image, SDL error: %s", SDL_GetError());
        goto err;
    }

    SDL_SetTextureBlendMode(scaled_texture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(renderer, scaled_texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, image->texture, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);

    struct k_image *new_image = k__image_create(scaled_texture);
    if (NULL == new_image) {
        k_log_error("Failed to scale image");
        goto err;
    }

    return new_image;

err:
    if (NULL != scaled_texture)
        SDL_DestroyTexture(scaled_texture);

    return NULL;
}
