#include "../image/k_image.h"

#include "./k_sprite.h"

int k_sprite_draw(struct k_sprite *sprite, size_t frame_idx, float dst_x, float dst_y) {

    if (NULL == sprite)
        return -1;

    if (sprite->frames_num <= frame_idx)
        return -1;

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

int k_sprite_draw_ex(struct k_sprite *sprite, size_t frame_idx, struct k_sprite_draw_options *options) {

    if (NULL == sprite || NULL == options)
        return -1;

    if (sprite->frames_num <= frame_idx)
        return -1;

    if (options->scaled_w <= 0 || options->scaled_h <= 0) {
        /* 若伸缩变化将图片宽高压缩至不可见，则不需要执行真正的绘制操作 */
        return 0;
    }

    struct k_sprite_frame *frame = &sprite->frames[frame_idx];

    struct k_int_rect src;
    src.x = frame->offset_x;
    src.y = frame->offset_y;
    src.w = sprite->sprite_w;
    src.h = sprite->sprite_h;

    float scala_x = (float)options->scaled_w / (float)(sprite->sprite_w);
    float scala_y = (float)options->scaled_h / (float)(sprite->sprite_h);

    float scaled_origin_x = sprite->origin_x * scala_x;
    float scaled_origin_y = sprite->origin_y * scala_y;

    struct k_image_draw_options opts;
    opts.src_rect = &src;


    // opts.dst_x    = options->x - scaled_origin_x;
    // opts.dst_y    = options->y - scaled_origin_y;

    if (options->flip_x) {
        opts.dst_x   = options->x - scala_x * ((float)options->scaled_w - sprite->origin_x);
        opts.pivot_x = scala_x * ((float)options->scaled_w - sprite->origin_x);
    } else {
        opts.dst_x   = options->x - scala_x * sprite->origin_x;
        opts.pivot_x = scala_x * sprite->origin_x;
    }

    if (options->flip_y) {
        opts.dst_y   = options->y - scala_y * ((float)options->scaled_h - sprite->origin_y);
        opts.pivot_y = scala_y * ((float)options->scaled_h - sprite->origin_y);
    } else {
        opts.dst_y   = options->y - scala_y * sprite->origin_y;
        opts.pivot_y = scala_y * sprite->origin_y;
    }

    opts.dst_w    = options->scaled_w;
    opts.dst_h    = options->scaled_h;
    opts.angle    = options->angle;
    opts.flip_x   = options->flip_x;
    opts.flip_y   = options->flip_y;

    return k__image_draw_ex(frame->image, &opts);
}
