#ifndef K_GAME_SPRITE_H
#define K_GAME_SPRITE_H

#include <stddef.h>

struct k_image;

/**
 * \brief 精灵
 *
 * 精灵可以是一张静态图片，或是一组连续的动画帧。
 * 用来表示游戏中的各种可见元素，例如：角色、敌人、道具等。
 */
struct k_sprite;

/* region [sprite_create] */

/** \brief 用于创建精灵帧的配置 */
struct k_sprite_frame_config {

    /**
     * \brief 精灵帧引用的图片
     *
     * 指定精灵帧所引用的图片。
     *
     * 精灵仅持有图片指针，而不复制图片。
     * 若要销毁图片，请确保该图片未被精灵引用，以避免悬空指针。
     */
    struct k_image *image;

    /**
     * \brief 精灵帧在原图上的偏移，单位：像素
     *
     * 精灵帧通常引用的是一个大图（图集）中的局部区域。
     * 该偏移量指定了精灵帧的左上角在原图中的坐标位置。
     */
    int offset_x;
    int offset_y;

    /**
     * \brief 帧延迟，单位：毫秒
     *
     * 指定当前帧在精灵的动画序列中的显示时间。
     * 该值决定了当前帧需要停留多久才能切换到下一帧。
     */
    int delay;
};

/** \brief 用于创建精灵的配置 */
struct k_sprite_config {

    /**
     * \brief 精灵宽高，单位：像素
     *
     * 精灵动画的每一帧都采用固定的宽高。
     */
    int sprite_w;
    int sprite_h;

    /**
     * \brief 精灵原点
     *
     * 精灵原点是精灵的基准点，用于定义精灵的变换中心和定位参考点。
     * 原点坐标是相对于精灵左上角的偏移量。
     */
    float origin_x;
    float origin_y;

    /**
     * \brief 精灵帧
     *
     * 指向精灵帧配置数组的指针，用于定义精灵每一帧的属性。
     * 数组长度由 `frames_num` 指定。
     *
     * 创建精灵时 k_game 会复制该数组。
     */
    struct k_sprite_frame_config *frames;

    /**
     * \brief 精灵帧的数量
     *
     * 精灵必须至少有一个帧。
     */
    int frames_num;
};

/**
 * \brief 创建精灵
 *
 * 若创建成功，函数返回精灵指针，否则返回 `NULL`。
 */
struct k_sprite *k_sprite_create(const struct k_sprite_config *config);

/**
 * \brief 销毁精灵
 *
 * 若 `sprite` 为 `NULL`，则函数不做任何事。
 */
void k_sprite_destroy(struct k_sprite *sprite);

/**
 * \brief 设置精灵的名字
 *
 * 精灵名字是可选的，默认情况下精灵没有名字。
 *
 * 你可以为精灵设置唯一名字，k_game 将基于该名字为精灵建立索引，
 * 之后可使用 `k_sprite_find()` 根据名字查找精灵。
 *
 * 若名字设为空字符串 "" 或 `NULL`，则清除名字，并删除索引。
 *
 * 注意：k_game 不会复制名字，而仅是保存字符串指针。
 * 请确保该字符串的内存段在整个使用期间有效且不被修改。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_set_name(struct k_sprite *sprite, const char *sprite_name);

/**
 * \brief 通过精灵名称查找精灵
 *
 * 若找到，函数返回精灵指针，否则返回 `NULL`。
 */
struct k_sprite *k_sprite_find(const char *sprite_name);

/* endregion */

/* region [sprite_get] */

/** \brief 获取精灵的宽 */
int k_sprite_get_width(struct k_sprite *sprite);

/** \brief 获取精灵的高 */
int k_sprite_get_height(struct k_sprite *sprite);

/** \brief 获取精灵原点的水平偏移量 */
float k_sprite_get_origin_x(struct k_sprite *sprite);

/** \brief 获取精灵原点的竖直偏移量 */
float k_sprite_get_origin_y(struct k_sprite *sprite);

/** \brief 获取精灵的帧数 */
int k_sprite_get_frames_num(struct k_sprite *sprite);

/** \brief 获取精灵帧的延时 */
int k_sprite_get_frame_delay(struct k_sprite *sprite, size_t frame_idx);

/* endregion */

/* region [sprite_draw] */

/** \brief 用于指定在绘制精灵帧时应用的变换效果 */
struct k_sprite_draw_options {

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
    uint8_t flip_x;
    uint8_t flip_y;
};

/**
 * \brief 绘制精灵帧
 *
 * `frame_idx` 指定要绘制帧的索引，`x` 和 `y` 指定精灵原点的绘制位置坐标。
 * `options` 用于执行缩放、旋转和翻转变换，若为 `NULL` 则不应用任何变换。
 *
 * 请确保 `sprite` 是有效的指针，`frame_idx` 是有效的索引值。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_draw(struct k_sprite *sprite, size_t frame_idx, float x, float y, struct k_sprite_draw_options *options);

/* endregion */

#endif
