#include "k_game/image_draw.h"
#include "k_game_sprite.h"
#include "k_game/sprite_create.h"
#include "k_game/sprite_draw.h"

int k__draw_sprite_frame(struct k_sprite *sprite, int dst_x, int dst_y, size_t frame_idx) {

    struct k_sprite_frame *frame = &sprite->frames[frame_idx];

    struct k_int_rect src;
    src.x = frame->offset_x;
    src.y = frame->offset_y;
    src.w = sprite->sprite_w;
    src.h = sprite->sprite_h;

    return k__draw_image(frame->image, &src, dst_x, dst_y);
}

int k_draw_sprite_frame(struct k_sprite *sprite, int x, int y, size_t frame_idx) {
    /* TODO: assert( NULL != sprite ) */
    /* TODO: assert currently is in draw callback */

    return k__draw_sprite_frame(sprite, x, y, frame_idx);
}
