#include "k_game/core/k_image.h"
#include "./k_image.h"

int k_image_get_w(struct k_image *image) {
    return image->image_w;
}

int k_image_get_h(struct k_image *image) {
    return image->image_h;
}
