#ifndef K_CANVAS_H
#define K_CANVAS_H

#include <stdint.h>

struct k_image;
struct k_sprite;

struct k_float_point {
    float x;
    float y;
};

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

void k_canvas_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

int k_canvas_draw_point(float x, float y);

int k_canvas_draw_line(float x1, float y1, float x2, float y2);

int k_canvas_draw_lines(const struct k_float_point *points, size_t points_num);

int k_canvas_draw_rect(float x, float y, float w, float h);

int k_canvas_draw_circle(float cx, float cy, float r);

/* region [draw_image] */

/** \brief 用于指定在绘制图片时应用的变换效果 */
struct k_canvas_draw_image_options {

    /**
     * \brief 缩放绘制图片
     *
     * `scaled_w` 和 `scaled_h` 分别指定目标宽高，
     * 绘制时，将图片的宽高拉伸或压缩至该值。
     *
     * 若目标宽高之一为 0 或负值，则图片被压缩至不可见。
     * 若不希望缩放图片，请指定目标宽高为图片原本的宽高，
     * 使用 `k_image_get_width()` 和 `k_image_get_height()`。
     */
    float scaled_w;
    float scaled_h;

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
 * \brief 在房间内绘制图片
 *
 * `src_rect` 指定源矩形区域，若为 `NULL` 则绘制整张图片，否则绘制原图中指定的裁剪部分。
 * `x` 和 `y` 指定图片左上角在房间中的坐标。
 * `options` 用于执行缩放、旋转和镜像翻转变换，若为 `NULL` 则不应用任何变换。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_canvas_draw_image(struct k_image *image, const struct k_int_rect *src_rect, float x, float y, struct k_canvas_draw_image_options *options);

/* endregion */

/* region [draw_sprite] */

/** \brief 用于指定在绘制精灵帧时应用的变换效果 */
struct k_canvas_draw_sprite_options {

    /**
     * \brief 缩放绘制精灵
     *
     * `scaled_w` 和 `scaled_h` 分别指定目标宽高，
     * 绘制时，精灵图的宽高将被拉伸或压缩至该值。
     * 伸缩变换的原点为精灵的原点。
     *
     * 若目标宽高之一为 0 或负值，则精灵图被压缩至不可见。
     * 若不希望缩放精灵，请指定目标宽高为精灵原本的宽高，
     * 使用 `k_sprite_get_width()` 和 `k_sprite_get_height()`。
     */
    float scaled_w;
    float scaled_h;

    /**
     * \brief 旋转绘制精灵
     *
     * `angle` 指定旋转角度，单位采用角度制，顺时针方向为正方向。
     * 旋转变换的原点为精灵的原点。
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
     * 翻转变换的原点为精灵的原点。
     */
    int flip_x;
    int flip_y;
};

/**
 * \brief 在房间内绘制精灵帧
 *
 * `frame_idx` 指定要绘制帧的索引。`x` 和 `y` 指定精灵原点的在房间中的坐标。
 * `options` 用于执行缩放、旋转和翻转变换，若为 `NULL` 则不应用任何变换。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_canvas_draw_sprite(struct k_sprite *sprite, size_t frame_idx, float x, float y, struct k_canvas_draw_sprite_options *options);

/* endregion */

#endif
