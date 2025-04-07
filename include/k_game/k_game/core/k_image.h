#ifndef K_IMAGE_H
#define K_IMAGE_H

#include <stdint.h>

struct k_int_rect {
    int x;
    int y;
    int w;
    int h;
};

struct k_float_rect {
    float x;
    float y;
    float w;
    float h;
};

/**
 * TODO docs
 */
struct k_image;

/* region [image_load] */

struct k_image *k_image_load(const char *filepath);

/**
 * \brief 释放图片资源
 *
 * 释放图片资源前，请确保该图片没有被引用。
 * 若有精灵引用了该图片，释放图片后，精灵持有的图片指针将变为悬空指针。
 */
void k_image_release(struct k_image *image);

/* endregion */

/* region [image_find] */

int k_image_set_name(struct k_image *image, const char *image_name);

struct k_image *k_image_find(const char *image_name);

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
     * \brief 镜像翻转绘制图片
     *
     * `flip_x` 和 `flip_y` 分别指定是否启用水平、竖直镜像翻转,
     * 0 表示不启用，非 0 则启用。
     *
     * 翻转变换的原点为目标矩形的中心点。
     */
    int flip_x;
    int flip_y;
};

/**
 * \brief 绘制图片
 *
 * 该函数用于在房间内绘制一张图片。
 * `src_rect` 指定源矩形区域，若为 `NULL` 则绘制整张图片，否则绘制原图中指定的裁剪部分。
 * `dst_rect` 指定目标矩形区域，函数将图片绘制到房间中的指定位置，并进行必要的缩放。
 * `options` 用于执行旋转和镜像翻转变换，若为 `NULL` 则不应用任何变换。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_image_draw(struct k_image *image, const struct k_int_rect *src_rect, const struct k_float_rect *dst_rect, struct k_image_draw_options *options);

/* endregion */

#endif
