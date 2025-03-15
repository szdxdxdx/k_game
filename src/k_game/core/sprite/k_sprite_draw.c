#include "../image/k_image.h"

#include "./k_sprite.h"

int k__sprite_draw_frame(struct k_sprite *sprite, size_t frame_idx, float dst_x, float dst_y) {

    struct k_sprite_frame *frame = &sprite->frames[frame_idx];

    struct k_int_rect src;
    src.x = frame->offset_x;
    src.y = frame->offset_y;
    src.w = sprite->sprite_w;
    src.h = sprite->sprite_h;

    float x = dst_x - sprite->origin_x;
    float y = dst_y - sprite->origin_y;
    return k__image_draw(frame->image, &src, x, y);
}

int k_sprite_draw_frame(struct k_sprite *sprite, size_t frame_idx, float x, float y) {
    /* TODO: assert( NULL != sprite ) */
    /* TODO: assert currently is in draw callback */
    return k__sprite_draw_frame(sprite, frame_idx, x, y);
}
