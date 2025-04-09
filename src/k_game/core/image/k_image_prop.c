#include "k_game/core/k_image.h"
#include "./k_image.h"

int k_image_get_width(struct k_image *image) {
    return image->image_w;
}

int k_image_get_height(struct k_image *image) {
    return image->image_h;
}
