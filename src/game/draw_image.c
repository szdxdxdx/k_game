#include "SDL.h"
#include "SDL_image.h"

#include "k/alloc.h"
#include "k/list.h"
#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

struct k_image {
    struct k_list_node iter_node;
    SDL_Texture *texture;
    int w, h;
};

struct k_image *k_load_image(const char *path) {

    struct k_image *img = k_malloc(sizeof(struct k_image));
    if (NULL == img)
        goto malloc_failed;

    img->texture = IMG_LoadTexture(k__game->renderer, path);
    if (NULL == img->texture) {
        k_log_error("IMG_LoadTexture() failed: %s", IMG_GetError());
        goto load_img_failed;
    }

    if (0 != SDL_QueryTexture(img->texture, NULL, NULL, &img->w, &img->h)) {
        k_log_error("SDL_QueryTexture() failed: %s", IMG_GetError());
        goto query_img_failed;
    }

    return img;

query_img_failed:
load_img_failed:
    k_free(img);

malloc_failed:
    return NULL;
}

int k_draw_image(struct k_image *img, int x, int y) {

    SDL_Rect dst = { .x = x, .y = y, .w = img->w, .h = img->h, };

    int result = SDL_RenderCopy(k__game->renderer, img->texture, NULL, &dst);
    if (0 == result)
        return 0;

    k_log_error("SDL_RenderCopy() failed: %s", IMG_GetError());
    return -1;
}
