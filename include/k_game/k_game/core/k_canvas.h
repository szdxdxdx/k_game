#ifndef K_CANVAS_H
#define K_CANVAS_H

#include <stdint.h>

#include "./k_game_fwd.h"

/* region [set_color] */

/**
 * \brief 设置画笔的颜色
 *
 * 更改画笔的颜色为 `(r, g, b, a)` 所指定的颜色值，
 * `r, g, b, a` 分别指定红色通道、绿色通道、蓝色通道和透明度通道的值。
 * 每个通道的取值范围是 0 ~ 255，用十六进制表示法则是 0x00 ~ 0xff。
 */
void k_canvas_set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/**
 * \brief 获取画笔的颜色
 *
 * 函数通过参数列表返回当前画笔的颜色，若传 `NULL` 则不接受该出参。
 * `r, g, b, a` 分别返回红色通道、绿色通道、蓝色通道和透明度通道的值。
 */
void k_canvas_get_draw_color(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);

/**
 * \brief 设置画笔的颜色
 *
 * 更改画笔颜色为 `hex_rgba` 所指定的十六进制 `0xRRGGBBAA` 格式的颜色值。
 */
static inline void k_canvas_set_draw_color_rgba(uint32_t hex_rgba) {
    uint8_t r = 0xff & (hex_rgba >> 24);
    uint8_t g = 0xff & (hex_rgba >> 16);
    uint8_t b = 0xff & (hex_rgba >> 8);
    uint8_t a = 0xff & (hex_rgba);
    k_canvas_set_draw_color(r, g, b, a);
}

/**
 * \brief 获取画笔的颜色
 *
 * 函数返回当前画笔的颜色值，为十六进制 `0xRRGGBBAA` 格式。
 */
static inline uint32_t k_canvas_get_draw_color_rgba(void) {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    k_canvas_get_draw_color(&r, &g, &b, &a);
    return ((uint32_t)r << 24)
         | ((uint32_t)g << 16)
         | ((uint32_t)b << 8)
         | ((uint32_t)a);
}

/* endregion */

/* region [draw_graphics] */

/**
 * \brief 清空房间画布内容
 *
 * 用当前画笔的颜色覆盖房间。若成功，函数返回 0，否则返回非 0。
 */
int k_canvas_room_clear(void);

/**
 * \brief 在房间内绘制一个点
 *
 * `(x, y)` 为点的坐标。
 * 若绘制成功，函数返回 0，否则返回非 0。
 */
int k_canvas_room_draw_point(float x, float y);

/**
 * \brief 在房间内绘制多个点
 *
 * `points` 为点坐标的数组，`points_num` 指定数组长度。
 * 若绘制成功，函数返回 0，否则返回非 0。
 */
int k_canvas_room_draw_points(const struct k_float_point *points, size_t points_num);

/**
 * \brief 在房间内绘制一条线段
 *
 * `(x1, y1)` 和 `(x2, y2)` 为线段的两个端点坐标。
 * 若绘制成功，函数返回 0，否则返回非 0。
 */
int k_canvas_room_draw_line(float x1, float y1, float x2, float y2);

/**
 * \brief 在房间内连续绘制多条线段
 *
 * `points` 为点坐标的数组，`points_num` 指定数组长度。
 * 函数将一次将点连接起来绘制线段，共绘制 `points_num - 1` 条线段。
 * 若 `points` 中的点个数少于 2，则不绘制任何内容。
 *
 * 若绘制成功，函数返回 0，否则返回非 0。
 *
 * 提示：若要绘制任意多边形，可以令 `points` 中始末两点的坐标相同。
 */
int k_canvas_room_draw_lines(const struct k_float_point *points, size_t points_num);

/**
 * \brief 在房间内绘制一个轴对齐矩形
 *
 * `(x, y)` 为矩形的左上角坐标，`w` 和 `h` 为矩形的宽和高。
 * 若宽高 `w` 或 `h` 为 0 或负值，则不绘制任何内容。
 *
 * 若绘制成功，函数返回 0，否则返回非 0。
 *
 * 提示：若想通过一组对角坐标来绘制矩形，或是绘制旋转的矩形，
 * 你可以使用 `k_canvas_room_draw_lines()` 来绘制矩形的边。
 */
int k_canvas_room_draw_rect(float x, float y, float w, float h);

/**
 * \brief 在房间内绘制一个填充的轴对齐矩形
 *
 * `(x, y)` 为矩形的左上角坐标，`w` 和 `h` 为矩形的宽和高。
 * 若宽高 `w` 或 `h` 为 0 或负值，则不绘制任何内容。
 *
 * 若绘制成功，函数返回 0，否则返回非 0。
 */
int k_canvas_room_fill_rect(float x, float y, float w, float h);

/**
 * \brief 在房间内绘制一个圆
 *
 * `(cx, cy)` 为圆的圆心坐标，`r` 为半径。
 * 若半径 `r` 为 0 或负值，则不绘制任何内容。
 *
 * 若绘制成功，函数返回 0，否则返回非 0。
 */
int k_canvas_room_draw_circle(float cx, float cy, float r);

/* endregion */

/* region [draw_image] */

struct k_canvas_draw_image_options;

/**
 * \brief 在房间内绘制一张图片
 *
 * `src_rect` 指定源矩形区域，若为 `NULL` 则绘制整张图片，否则绘制原图中指定的裁剪部分。
 * 若 `src_rect` 指定裁剪部分的宽高为 0 或负数，则不绘制任何内容。
 *
 * `x` 和 `y` 指定图片左上角在房间中的坐标。
 * `options` 用于执行缩放、旋转和镜像翻转变换，若为 `NULL` 则不应用任何变换。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_canvas_room_draw_image(struct k_image *image, const struct k_int_rect *src_rect, float x, float y, struct k_canvas_draw_image_options *options);

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

/* endregion */

/* region [draw_sprite] */

struct k_canvas_draw_sprite_options;

/**
 * \brief 在房间内绘制一张精灵帧
 *
 * `frame_idx` 指定要绘制帧的索引。`x` 和 `y` 指定精灵原点的在房间中的坐标。
 * `options` 用于执行缩放、旋转和翻转变换，若为 `NULL` 则不应用任何变换。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_canvas_room_draw_sprite(struct k_sprite *sprite, size_t frame_idx, float x, float y, struct k_canvas_draw_sprite_options *options);

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

/* endregion */

/* region [debug_z_index] */

#define K_DEBUG_Z_GROUP 666666
#define K_DEBUG_Z_LAYER 0

/* endregion */

#endif
