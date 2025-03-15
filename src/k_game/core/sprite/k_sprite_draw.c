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

int k_sprite_draw(struct k_sprite *sprite, size_t frame_idx, float x, float y) {

    if (NULL == sprite)
        return -1;
    if (sprite->frames_num <= frame_idx)
        return -1;

    /* [?] assert currently is in draw callback */

    /* [?] if (not in camera view) return 0; */

    return k__sprite_draw_frame(sprite, frame_idx, x, y);
}

int k__sprite_draw_EX(struct k_sprite *sprite, size_t frame_idx, struct k_sprite_draw_options *options) {

    struct k_sprite_frame *frame = &sprite->frames[frame_idx];

    struct k_int_rect src;
    src.x = frame->offset_x;
    src.y = frame->offset_y;
    src.w = sprite->sprite_w;
    src.h = sprite->sprite_h;

    struct k_image_draw_options opt;
    opt.src_rect        = &src;
    opt.dst_x           = options->x - sprite->origin_x;
    opt.dst_y           = options->y - sprite->origin_y;
    opt.dst_w           = options->dst_w;
    opt.dst_h           = options->dst_h;  /* TODO (w > 0 && h > 0) */
    opt.angle           = options->angle;
    opt.pivot_x         = sprite->origin_x;
    opt.pivot_y         = sprite->origin_y;
    opt.horizontal_flip = options->horizontal_flip;
    opt.vertical_flip   = options->vertical_flip;

    return k__image_draw_EX(frame->image, &opt);
}

int k_sprite_draw_EX(struct k_sprite *sprite, size_t frame_idx, struct k_sprite_draw_options *options) {

    if (NULL == sprite || NULL == options)
        return -1;
    if (options->dst_w <= 0 || options->dst_h <= 0)
        return -1;
    if (sprite->frames_num <= frame_idx)
        return -1;

    /* [?] assert currently is in draw callback */

    /* [?] if (not in camera view) return 0; */

    return k__sprite_draw_EX(sprite, frame_idx, options);
}
