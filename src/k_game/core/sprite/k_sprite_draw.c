#include "../image/k_image.h"

#include "./k_sprite.h"

int k_sprite_draw(struct k_sprite *sprite, size_t frame_idx, float x, float y, struct k_sprite_draw_options *options) {

    if (NULL == sprite)
        return -1;

    if (sprite->frames_num <= frame_idx)
        return -1;

    struct k_sprite_frame *frame = &sprite->frames[frame_idx];

    /* 不应用变换，使用简单绘制 */
    if (NULL == options) {

        struct k_int_rect src;
        src.x = frame->offset_x;
        src.y = frame->offset_y;
        src.w = sprite->sprite_w;
        src.h = sprite->sprite_h;

        struct k_image_draw_options opts;
        opts.src_rect = &src;
        opts.dst_x    = x - sprite->origin_x;
        opts.dst_y    = y - sprite->origin_y;
        opts.scaled_w = sprite->sprite_w;
        opts.scaled_h = sprite->sprite_h;
        opts.angle    = 0.0f;
        opts.pivot_x  = sprite->origin_x;
        opts.pivot_y  = sprite->origin_y;
        opts.flip_x   = 0;
        opts.flip_y   = 0;

        return k_image_draw(frame->image, &opts);
    }

    /* 应用变换，使用复杂绘制 */
    else {
        if (options->scaled_w <= 0 || options->scaled_h <= 0)
            return 0;

        struct k_int_rect src;
        src.x = frame->offset_x;
        src.y = frame->offset_y;
        src.w = sprite->sprite_w;
        src.h = sprite->sprite_h;

        /* 将精灵原点移动到【经过伸缩、翻转】变换后的图片上 */
        float scala_x  = (float)options->scaled_w / (float)sprite->sprite_w;
        float scala_y  = (float)options->scaled_h / (float)sprite->sprite_h;
        float origin_x = scala_x * (options->flip_x ? (float)options->scaled_w - sprite->origin_x : (float)sprite->origin_x);
        float origin_y = scala_y * (options->flip_y ? (float)options->scaled_h - sprite->origin_y : (float)sprite->origin_y);

        struct k_image_draw_options opts;
        opts.src_rect = &src;
        opts.dst_x    = x - origin_x;
        opts.dst_y    = y - origin_y;
        opts.scaled_w = options->scaled_w;
        opts.scaled_h = options->scaled_h;
        opts.angle    = options->angle;
        opts.pivot_x  = origin_x;
        opts.pivot_y  = origin_y;
        opts.flip_x   = options->flip_x;
        opts.flip_y   = options->flip_y;

        return k_image_draw(frame->image, &opts);
    }
}
