#include <stddef.h>

#include "SDL_image.h"

#include "k_game.h"

#include "../k_SDL/k_SDL.h"
#include "./k_image.h"

static struct k_asset_registry image_registry;

/* region [image_create] */

struct k_image *k__image_create(SDL_Texture *texture) {

    struct k_image *image = k_malloc(sizeof(struct k_image));
    if (NULL == image)
        goto err;

    image->texture = texture;
    SDL_QueryTexture(texture, NULL, NULL, &image->image_w, &image->image_h);

    k__asset_registry_add(&image_registry, &image->registry_node);
    return image;

err:
    return NULL;
}

void k_image_release(struct k_image *image) {

    if (NULL == image)
        return;

    SDL_DestroyTexture(image->texture);

    k__asset_registry_del(&image->registry_node);
    k_free(image);
}

/* endregion */

/* region [image_registry] */

int k__image_registry_init(void) {
    return k__asset_registry_init(&image_registry);
}

static void fn_release_asset(struct k_asset_registry_node *node) {
    struct k_image *image = container_of(node, struct k_image, registry_node);
    k_image_release(image);
}

void k__image_registry_cleanup(void) {
     k__asset_registry_cleanup(&image_registry, fn_release_asset);
}

int k_image_set_name(struct k_image *image, const char *image_name) {
    return k__asset_set_name(&image_registry, &image->registry_node, image_name);
}

struct k_image *k_image_find(const char *image_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&image_registry, image_name);
    if (NULL == registry_node)
        return NULL;

    struct k_image *image = container_of(registry_node, struct k_image, registry_node);
    return image;
}

/* endregion */

/* region [image_load] */

struct k_image *k_image_load(const char *filepath) {

    if (NULL == filepath || '\0' == filepath[0])
        goto err;

    SDL_Texture *texture = IMG_LoadTexture(k__window.renderer, filepath);
    if (NULL == texture)
        goto err;

    struct k_image *image = k__image_create(texture);
    if (NULL == image) {
        SDL_DestroyTexture(texture);
        goto err;
    }

    return image;

err:
    return NULL;
}

/* endregion */

/* region [image_get] */

int k_image_get_width(struct k_image *image) {
    return image->image_w;
}

int k_image_get_height(struct k_image *image) {
    return image->image_h;
}

/* endregion */
