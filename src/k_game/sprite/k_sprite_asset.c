#include "./k_sprite.h"

int k_sprite_get_width(struct k_sprite *sprite) {
    return sprite->sprite_w;
}

int k_sprite_get_height(struct k_sprite *sprite) {
    return sprite->sprite_h;
}
