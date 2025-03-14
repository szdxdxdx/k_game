#include "k_log.h"

#include "../k_SDL/k_SDL.h"

#include "./k_image.h"

struct k_image *k_image_scale(struct k_image *image, int scaled_w, int scaled_h) {

    if (NULL == image || scaled_w <= 0 || scaled_h <= 0)
        return NULL;

    SDL_Renderer *renderer = k__window.renderer;

    Uint32 format = SDL_PIXELFORMAT_ARGB8888;
    int access = SDL_TEXTUREACCESS_TARGET;
    SDL_Texture* scaled_texture = SDL_CreateTexture(renderer, format, access, scaled_w, scaled_h);
    if (NULL == scaled_texture) {
        k_log_error("SDL_CreateTexture(): %s", SDL_GetError());
        goto err;
    }

    SDL_SetRenderTarget(renderer, scaled_texture);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // SDL_Rect destRect;
    // destRect.x = 0;
    // destRect.y = 0;
    // destRect.w = scaled_w;
    // destRect.h = scaled_h;

    SDL_RenderCopy(renderer, image->texture, NULL, NULL /*&destRect*/ );
    SDL_SetRenderTarget(renderer, NULL);

    struct k_image *new_image = k__image_create(scaled_texture);
    if (NULL == new_image) {
        SDL_DestroyTexture(scaled_texture);
        goto err;
    }

    return new_image;

err:
    k_log_error("Failed to Scale image");
    return NULL;
}
