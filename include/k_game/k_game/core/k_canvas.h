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

/* region [debug_z_index] */

#define K_DEBUG_Z_GROUP 666666
#define K_DEBUG_Z_LAYER 0

/* endregion */

/* region [set_color] */

/**
 * \brief 设置画笔的颜色
 *
 * 更改画笔的颜色为 `(r, g, b, a)` 所指定的颜色值，
 * 其中 `r, g, b` 分别指定红色、绿色、蓝色通道的值，
 * `a` 指定透明度通道的值，每个通道的取值范围是 0 ~ 255。
 *
 * 例如 `(r, g, b, a)`：
 * - 嫣汐橙 `(255, 102,   0, 255)` 或 `(0xff, 0f66, 0x00, 0xff)`
 * - 初音绿 `( 57, 197, 187, 255)` 或 `(0x39, 0xc5, 0xbb, 0xff)`
 * - 阿绫红 `(238,   0,   0, 255)` 或 `(0xee, 0x00, 0x00, 0xff)`
 * - 天依蓝 `(102, 204, 255, 255)` 或 `(0x66, 0xcc, 0xff, 0xff)`
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
 * 更改画笔的颜色为 `hex_rgba` 所指定十六进制 `0xRRGGBBAA` 格式的颜色值。
 * 例如：
 * - 嫣汐橙 `0xff6600ff`
 * - 初音绿 `0x39c5bbff`
 * - 阿绫红 `0xee0000ff`
 * - 天依蓝 `0x66ccffff`
 */
void k_canvas_set_draw_color_rgba(uint32_t hex_rgba);

/**
 * \brief 获取画笔的颜色
 *
 * 函数返回当前画笔的颜色值，为十六进制 `0xRRGGBBAA` 格式。
 */
uint32_t k_canvas_get_draw_color_rgba(void);

/* endregion */

/* region [draw_graphics] */

int k_canvas_draw_point(float x, float y);

int k_canvas_draw_points(const struct k_float_point *points, size_t points_num);

int k_canvas_draw_line(float x1, float y1, float x2, float y2);

int k_canvas_draw_lines(const struct k_float_point *points, size_t points_num);

int k_canvas_draw_rect(float x, float y, float w, float h);

int k_canvas_fill_rect(float x, float y, float w, float h);

int k_canvas_draw_circle(float cx, float cy, float r);

/* endregion */

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
