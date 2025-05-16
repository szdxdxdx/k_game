#ifndef K_SPRITE_RENDERER_H
#define K_SPRITE_RENDERER_H

#include "k_game/core/k_game_fwd.h"

/**
 * \brief 精灵渲染器
 *
 * 一般来说，游戏对象至少关联有一个精灵作为其外观表现。
 *
 * k_sprite_renderer 的本质是 k_component。
 * 给对象挂载上精灵渲染器，并指定引用的精灵图，
 * 渲染器能重复循环绘制精灵帧，形成动画效果。
 */
struct k_sprite_renderer;

/* region [object_add_sprite_renderer] */

/** \brief 用于初始化精灵渲染器的配置参数 */
struct k_sprite_renderer_config {

    /** \brief 渲染器关联的坐标 */
    float *x;
    float *y;

    /**
     * \brief 渲染器引用的精灵
     *
     * 指定渲染器初始化时引用的精灵。若为 `NULL`，则不引用任何精灵。
     *
     * 渲染器仅持有精灵的指针，不复制精灵数据，不管理精灵的生命周期。
     * 若要销毁精灵，请确保该精灵未被任何渲染器引用，以避免悬空指针。
     */
    struct k_sprite *sprite;

    /** \brief 渲染器的绘制深度 */
    int z_group;
    int z_layer;
};

/**
 * \brief 给对象挂载一个精灵渲染器
 *
 * 若成功，函数返回渲染器的指针，否则返回 `NULL`。
 */
struct k_sprite_renderer *k_object_add_sprite_renderer(struct k_object *object, const struct k_sprite_renderer_config *config);

/**
 * \brief 移除对象上挂载的精灵渲染器
 *
 * 若 `renderer` 为 `NULL`，则函数立即返回。
 */
void k_object_del_sprite_renderer(struct k_sprite_renderer *renderer);

/* endregion */

/* region [transform] */

/* region [scale_x] */

/**
 * \brief 设置渲染器绘制精灵的宽度
 *
 * 渲染器将延水平方向缩放绘制精灵，变换原点为精灵的原点。
 * - 若 `scaled_w` 为正值，则精灵图宽度将被拉伸或压缩至该值。
 * - 若 `scaled_w` 为 0.0f 或负值，则将其设置为 0.0f，此时精灵图被压缩至不可见。
 *
 * 若要恢复原始尺寸，则应用 scale_x 变换，缩放比率为 1.0f
 * 设置了 scaled_w 则会覆盖 scale_x 变换。
 */
void k_sprite_renderer_set_scaled_w(struct k_sprite_renderer *renderer, float scaled_w);

/**
 * \brief 设置渲染器水平方向的缩放比率
 *
 * 渲染器将延水平方向按比率缩放精灵，变换原点为精灵的原点。
 * - 若 `scale_x` 为 1.0f，则按原图尺寸绘制。
 * - 若 `scale_x` 为 2.0f，则拉伸精灵图宽度到原图的两倍。
 * - 若 `scale_x` 为 0.5f，则压缩精灵图宽度到原图的一半。
 * - 若 `scale_x` 为 0.0f 或负值，则将其设置为 0.0f，此时精灵图被压缩至不可见。
 *
 * 设置了 scale_x 则会覆盖 scaled_w 变换。
 */
void k_sprite_renderer_scale_x(struct k_sprite_renderer *renderer, float scale_x);

/**
 * \brief 获取渲染器绘制精灵的宽度
 *
 * 函数返回渲染器绘制精灵时所使用的最终宽度。
 * - 若渲染器水平方向上未使用任何缩放，函数返回精灵本身的原始宽度（若未引用精灵则返回 0.0f）。
 * - 若渲染器应用了 scaled_w 变换，则函数返回该变换指定的宽度值。
 * - 若渲染器应用了 scale_x 变换，则是精灵宽度乘以缩放倍率（若未引用精灵则返回 0.0f）。
 */
float k_sprite_renderer_get_w(struct k_sprite_renderer *renderer);

/* endregion */

/* region [scale_y] */

/**
 * \brief 设置渲染器绘制精灵的高度
 *
 * 与 `k_sprite_renderer_set_scaled_w()` 类似。
 */
void k_sprite_renderer_set_scaled_h(struct k_sprite_renderer *renderer, float scaled_h);

/**
 * \brief 设置渲染器竖直方向的缩放比率
 *
 * 与 `k_sprite_renderer_scale_y()` 类似。
 */
void k_sprite_renderer_scale_y(struct k_sprite_renderer *renderer, float scale_y);

/**
 * \brief 获取渲染器绘制精灵的高度
 *
 * 与 `k_sprite_renderer_get_w()` 类似。
 */
float k_sprite_renderer_get_h(struct k_sprite_renderer *renderer);

/* endregion */

/* region [rotate] */

/**
 * \brief 设置渲染器绘制精灵的旋转角度
 *
 * 渲染器将旋转绘制精灵，变换原点为精灵的原点。
 * 旋转单位采用角度制，顺时针方向为正方向。
 */
void k_sprite_renderer_rotate(struct k_sprite_renderer *renderer, float angle);

/**
 * \brief 获取渲染器绘制精灵的旋转角度
 *
 * 函数返回渲染器绘制精灵的的旋转角度。
 */
float k_sprite_renderer_get_rotation(struct k_sprite_renderer *renderer);

/* endregion */

/* region [flip_x] */

/**
 * \brief 设置渲染器延水平方向镜像翻转绘制精灵
 *
 * 若 `flip` 为非 0，则启用水平镜像翻转，否则不启用。
 * 变换原点为精灵的原点。
 */
void k_sprite_renderer_flip_x(struct k_sprite_renderer *renderer, int flip);

/**
 * \brief 检查渲染器是否启用水平镜像翻转
 *
 * 若渲染器启用水平镜像翻转，则函数返回非 0，否则返回 0。
 */
int k_sprite_renderer_is_flipped_x(struct k_sprite_renderer *renderer);

/* endregion */

/* region [flip_y] */

/**
 * \brief 设置渲染器延竖直方向镜像翻转绘制精灵
 *
 * 与 `k_sprite_renderer_flip_x()` 类似。
 */
void k_sprite_renderer_flip_y(struct k_sprite_renderer *renderer, int flip);

/**
 * \brief 检查渲染器是否启用竖直镜像翻转
 *
 * 与 `k_sprite_renderer_is_flipped_x()` 类似。
 */
int k_sprite_renderer_is_flipped_y(struct k_sprite_renderer *renderer);

/* endregion */

/**
 * \brief 重置渲染器对精灵图的所有变换操作
 *
 * 重置宽高缩放倍率为 1.0f，旋转角度为 0.0f，取消镜像翻转。
 */
void k_sprite_renderer_reset_transforms(struct k_sprite_renderer *renderer);

/* endregion */

/* region [z_index] */

/**
 * \brief 更改渲染器的绘制深度
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_renderer_set_z_index(struct k_sprite_renderer *renderer, int z_group, int z_layer);

/**
 * \brief 更改渲染器的绘制深度（组）
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_renderer_set_z_group(struct k_sprite_renderer *renderer, int z_group);

/**
 * \brief 更改渲染器的绘制深度（层）
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_renderer_set_z_layer(struct k_sprite_renderer *renderer, int z_layer);

/** \brief 获取渲染器的绘制深度（组） */
int k_sprite_renderer_get_z_group(struct k_sprite_renderer *renderer);

/** \brief 获取渲染器的绘制深度（层） */
int k_sprite_renderer_get_z_layer(struct k_sprite_renderer *renderer);

/* endregion */

/* region [speed] */

/**
 * \brief 设置渲染器播放精灵动画的速度
 *
 * - 若 `speed` 为 1.0f，即按照精灵每帧定义的 `delay` 值原速播放。
 * - 若 `speed` 为 2.0f，则 2 倍速快速播放。
 * - 若 `speed` 为 0.5f，则 0.5 倍速慢速播放。
 * - 若 `speed` 为 0.0f 或负值，则将其设置为 0.0f，并暂停播放。
 */
void k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed);

/**
 * \brief 设置渲染器播放精灵动画的时长，单位：秒
 *
 * 设置渲染器的动画播放时长，本质是修改其播放速度。
 * 渲染器将计算出合适的播放倍速并自动应用，使其播放一轮动画所用时间为指定的 `duration` 秒。
 *
 * 若渲染器没有引用精灵，或是 `duration` 为 0.0f 或负值，则函数立即返回。
 */
void k_sprite_renderer_set_duration(struct k_sprite_renderer *renderer, float duration);

/**
 * \brief 获取渲染器播放精灵动画的速度
 *
 * 函数返回渲染器播放精灵动画的速度，若渲染器没有引用精灵则返回 0.0f。
 */
float k_sprite_renderer_get_speed(struct k_sprite_renderer *renderer);

/* endregion */

/* region [loop] */

/**
 * \brief 设置渲染器循环播放的次数
 *
 * 渲染器将循环播放精灵动画 `loop_count` 次，并在循环结束后停在最后一帧。
 *
 * 渲染器不支持无限播放，但指定循环播放的次数设为 `INT_MAX`，也能达到“无限”的效果。
 * 播放次数最少为 1，若指定循环的次数为 0 或负值，渲染器仍会播放 1 次动画。
 *
 * 更换渲染器引用的精灵后，渲染器会重置循环播放的次数为 `INT_MAX`。
 */
void k_sprite_renderer_set_loop_count(struct k_sprite_renderer *renderer, int loop_count);

/** \brief 获取渲染器循环播放的次数 */
int k_sprite_renderer_get_loop_count(struct k_sprite_renderer *renderer);

/**
 * \brief 设置渲染器每播放完一次动画后执行的回调函数
 *
 * 渲染器每播放完一次动画，便会执行一次该回调。
 *
 * 若指定 `fn_callback` 为 `NULL`，则移除当前的回调。
 * 更换渲染器引用的精灵后，渲染器也会移除该回调。
 */
void k_sprite_renderer_set_loop_callback(struct k_sprite_renderer *renderer, void (*on_loop_end)(struct k_object *object));

/* region [ref_sprite] */

/**
 * \brief 设置渲染器引用的精灵
 *
 * 若指定新的精灵为 `NULL`，则仅移除当前引用的精灵。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_renderer_set_sprite(struct k_sprite_renderer *renderer, struct k_sprite *sprite);

/**
 * \brief 获取渲染器当前引用的精灵
 *
 * 函数返回渲染器当前引用的精灵，若渲染器没有引用精灵则返回 `NULL`。
 */
struct k_sprite *k_sprite_renderer_get_sprite(struct k_sprite_renderer *renderer);

/* endregion */

/* endregion */

/* region [debug] */

/**
 * \brief 开启或关闭渲染器的调试显示功能
 *
 * 若 `debug` 为非 0 则启用调试，否则关闭。
 * 启用调试后，渲染器将在关联的坐标处绘制十字标记。
 * 若渲染器引用了精灵，则额外显示精灵图的边框。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_renderer_set_debug(struct k_sprite_renderer *renderer, int debug);

/* endregion */

#endif
