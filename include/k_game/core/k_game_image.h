#ifndef K_GAME_IMAGE_H
#define K_GAME_IMAGE_H

#include <stdint.h>

#include "./k_game_rect.h"

/**
 * TODO docs
 */
struct k_image;

/* region [image_load] */

struct k_image *k_image_load(const char *filepath);

int k_image_set_name(struct k_image *image, const char *image_name);

struct k_image *k_image_find(const char *image_name);

/**
 * \brief 释放图片资源
 *
 * 释放图片资源前，请确保该图片没有被引用。
 * 若有精灵引用了该图片，释放图片后，精灵持有的图片指针将变为悬空指针。
 */
void k_image_release(struct k_image *image);

/* endregion */

/* region [image_get] */

int k_image_get_width(struct k_image *image);

int k_image_get_height(struct k_image *image);

/* endregion */

/* region [image_transform] */

struct k_image *k_image_scale(struct k_image *image, int scaled_w, int scaled_h);

/* endregion */

/* region [image_draw] */

/** \brief 用于指定在图片时应用的变换效果 */
struct k_image_draw_options {

    const struct k_int_rect *src_rect;

    float dst_x;
    float dst_y;

    /**
     * \brief 缩放绘制图片
     *
     * `scaled_w` 和 `scaled_h` 分别指定目标宽高，
     * 绘制时，图片的宽高将被拉伸或压缩至该值。
     *
     * 若目标宽高之一为 0 或负值，则图片被压缩至不可见。
     * 若不希望缩放图片，则指定目标宽高为图片原本的宽高，
     * 使用 `k_image_get_width()` 和 `k_image_get_height()`。
     */
    int scaled_w;
    int scaled_h;

    /**
     * \brief 旋转绘制图片
     *
     * `angle` 指定旋转角度，单位采用角度制，顺时针方向为正方向，
     * 若不需要旋转，请将 `angle` 指定为 0.0f。
     *
     * 旋转变换的原点坐标为 `pivot_x` 和 `pivot_y`，相对于源矩形的左上角。
     */
    float angle;
    float pivot_x;
    float pivot_y;

    /**
     * \brief 翻转绘制图片
     *
     * `flip_x` 和 `flip_y` 分别指定是否启用水平、竖直翻转,
     * 0 表示不启用，非 0 则启用。
     *
     * 翻转变换的原点为目标矩形的中心点。
     */
    uint8_t flip_x;
    uint8_t flip_y;
};

/**
 * \brief 绘制图片
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_image_draw(struct k_image *image, struct k_image_draw_options *options);

/* endregion */

#endif
