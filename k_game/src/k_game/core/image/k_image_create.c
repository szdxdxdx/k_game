#include "k_game/core/k_mem_alloc.h"

#include "./k_image.h"
#include "./k_image_create.h"
#include "./k_image_registry.h"

struct k_image *k__image_create(SDL_Texture *texture) {

    struct k_image *image = k__mem_alloc(sizeof(struct k_image));
    if (NULL == image)
        return NULL;

    image->texture = texture;
    SDL_QueryTexture(texture, NULL, NULL, &image->image_w, &image->image_h);

    k__image_registry_add(image);
    return image;
}

void k__image_destroy(struct k_image *image) {

    SDL_DestroyTexture(image->texture);

    k__image_registry_del(image);
    k__mem_free(image);
}
