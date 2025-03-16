#ifndef K_GAME_SPRITE_H
#define K_GAME_SPRITE_H

#include <stddef.h>

#include "k_game_rect.h"

struct k_image;

/**
 * \brief 精灵
 *
 * TODO docs
 */
struct k_sprite;

/* region [sprite_create] */

/**
 * \brief 用于创建精灵的配置
 *
 * TODO docs
 */
struct k_sprite_frame_config {

    struct k_image *image;

    int offset_x;
    int offset_y;

    int delay;
};

/**
 * \brief 用于创建精灵的配置
 *
 * TODO docs
 */
struct k_sprite_config {

    int sprite_w;
    int sprite_h;

    float origin_x;
    float origin_y;

    struct k_sprite_frame_config *frames;

    int frames_num;
};

struct k_sprite *k_sprite_create(const struct k_sprite_config *config);

int k_sprite_set_name(struct k_sprite *sprite, const char *sprite_name);

struct k_sprite *k_sprite_find(const char *sprite_name);

/* endregion */

/* region [sprite_get] */

int k_sprite_get_width(struct k_sprite *sprite);

int k_sprite_get_height(struct k_sprite *sprite);

int k_sprite_get_frames_num(struct k_sprite *sprite);

int k_sprite_get_frame_delay(struct k_sprite *sprite, size_t frame_idx);

/* endregion */

/* region [sprite_draw] */

/** \brief 用于指定绘制精灵时的各种变换参数 */
struct k_sprite_draw_options {

    /**
     * \brief 缩放绘制精灵
     *
     * `scaled_w` 和 `scaled_h` 分别指定目标宽高，
     * 绘制时，精灵图的宽高将被拉伸或压缩至该值。
     * 伸缩变换的原点为精灵的中心。
     *
     * 若目标宽高其中之一为 0 或负值，则精灵图被压缩至不可见。
     * 若不希望缩放精灵，请指定目标宽高为精灵原本的宽高，
     * 使用 `k_sprite_get_width()` 和 `k_sprite_get_height()`。
     */
    int scaled_w;
    int scaled_h;

    /**
     * \brief 旋转绘制精灵
     *
     * `angle` 指定旋转角度，单位采用角度制，顺时针方向为正方向。
     * 旋转变换的原点为精灵的中心。
     *
     * 若不需要旋转，请将 `angle` 指定为 0.0f。
     */
    float angle;

    /**
     * \brief 翻转绘制精灵
     *
     * `flip_x` 和 `flip_y` 分别指定是否启用水平、竖直翻转,
     * 0 表示不启用，非 0 则启用。
     *
     * 翻转变换的原点为精灵的中心。
     */
    uint8_t flip_x;
    uint8_t flip_y;
};

/**
 * \brief 绘制精灵帧
 *
 * `options` 指定绘制的变换参数，若为 `NULL` 则不应用任何变换。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_draw(struct k_sprite *sprite, size_t frame_idx, float x, float y, struct k_sprite_draw_options *options);

/* endregion */

#endif
