#include "../image/image_draw.h"
#include "k_game_sprite.h"
#include "./sprite_asset.h"
#include "./sprite_draw.h"

int k__sprite_draw_frame(struct k_sprite *sprite, int dst_x, int dst_y, size_t frame_idx) {

    struct k_sprite_frame *frame = &sprite->frames[frame_idx];

    struct k_int_rect src;
    src.x = frame->offset_x;
    src.y = frame->offset_y;
    src.w = sprite->sprite_w;
    src.h = sprite->sprite_h;

    return k__image_draw(frame->image, &src, dst_x, dst_y);
}

int k_sprite_draw_frame(struct k_sprite *sprite, int x, int y, size_t frame_idx) {
    /* TODO: assert( NULL != sprite ) */
    /* TODO: assert currently is in draw callback */
    return k__sprite_draw_frame(sprite, x, y, frame_idx);
}
