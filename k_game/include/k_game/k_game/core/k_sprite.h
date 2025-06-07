#ifndef K_SPRITE_H
#define K_SPRITE_H

#include <stddef.h>

#include "./k_game_fwd.h"

struct k_sprite;

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
     *
     * 原点不一定位于精灵图的左上角。在实际游戏开发中，描述一个人物角色的位置时，
     * 往往是指它的躯干中心或脚下在房间中的坐标，而不是指素材图片的左上角在房间中的坐标，
     * 此时精灵图的原点便可以设在图中人物的躯干中心或脚下。
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
 * 若 `sprite` 为 `NULL`，则函数立即返回。
 */
void k_sprite_destroy(struct k_sprite *sprite);

/** \brief 获取精灵的宽 */
int k_sprite_get_w(struct k_sprite *sprite);

/** \brief 获取精灵的高 */
int k_sprite_get_h(struct k_sprite *sprite);

/** \brief 获取精灵原点的水平偏移量 */
float k_sprite_get_origin_x(struct k_sprite *sprite);

/** \brief 获取精灵原点的竖直偏移量 */
float k_sprite_get_origin_y(struct k_sprite *sprite);

/** \brief 获取精灵的帧数 */
int k_sprite_get_frames_num(struct k_sprite *sprite);

/** \brief 获取精灵帧的延时，单位：毫秒 */
int k_sprite_get_frame_delay(struct k_sprite *sprite, size_t frame_idx);

#endif
