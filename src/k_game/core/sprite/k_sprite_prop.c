#include "./k_sprite.h"

int k_sprite_get_width(struct k_sprite *sprite) {
    return sprite->sprite_w;
}

int k_sprite_get_height(struct k_sprite *sprite) {
    return sprite->sprite_h;
}

float k_sprite_get_origin_x(struct k_sprite *sprite) {
    return sprite->origin_x;
}

float k_sprite_get_origin_y(struct k_sprite *sprite) {
    return sprite->origin_y;
}

int k_sprite_get_frames_num(struct k_sprite *sprite) {
    return sprite->frames_num;
}

int k_sprite_get_frame_delay(struct k_sprite *sprite, size_t frame_idx) {
    return sprite->frames[frame_idx].delay;
}
