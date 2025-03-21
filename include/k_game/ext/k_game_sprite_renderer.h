#ifndef K_GAME_SPRITE_RENDERER_H
#define K_GAME_SPRITE_RENDERER_H

struct k_sprite;
struct k_object;

/**
 * \brief 精灵渲染器
 *
 * 精灵渲染器的本质是一个组件。
 *
 * 一般来说，对象至少关联有一个精灵作为其外观表现。
 * 给对象挂载上精灵渲染器组件，并指定引用的精灵图，
 * 渲染器能重复循环绘制精灵帧，形成动画效果。
 */
struct k_sprite_renderer;

/* region [object_add_sprite_renderer] */

/** \brief 用于初始化精灵渲染器的配置参数 */
struct k_sprite_renderer_config {

    /**
     * \brief 渲染器关联的坐标
     *
     * 对象的位置信息由对象自身维护，渲染器不存储坐标，而是存储坐标的指针。
     * 绘制精灵时，渲染器通过指针读取坐标，并在该位置绘制精灵。
     */
    float *x;
    float *y;

    /**
     * \brief 指定渲染器引用的精灵
     *
     * 指定渲染器初始化时引用的精灵。若为 `NULL`，则不引用任何精灵。
     *
     * 渲染器仅持有精灵的指针，不复制精灵数据，不管理精灵的生命周期。
     * 若要销毁精灵，请确保该精灵未被任何渲染器引用，以避免悬空指针。
     */
    struct k_sprite *sprite;

    /** \brief 指定渲染器的绘制深度 */
    int z_index;
};

/**
 * \brief 为对象添加一个精灵渲染器组件
 *
 * 若成功，函数返回渲染器组件指针，否则返回 `NULL`。
 */
struct k_sprite_renderer *k_object_add_sprite_renderer(struct k_object *object, const struct k_sprite_renderer_config *config);

/**
 * \brief 移除对象上挂载的精灵渲染器组件
 *
 * 若 `renderer` 为 `NULL`，则函数不做任何事。
 */
void k_object_del_sprite_renderer(struct k_sprite_renderer *renderer);

/* endregion */

/* region [sprite_renderer transform] */

/* region [scale_x] */

/**
 * \brief 设置渲染器绘制精灵的宽度
 *
 * 渲染器将延原精灵图的水平方向缩放精灵，变换原点为精灵的原点。
 *
 * - 若 `scaled_w` 为正值，则精灵图宽度将被拉伸或压缩至该值。
 * - 若 `scaled_w` 为 0 或负值，则将其设置为 0，此时精灵图被压缩至不可见。
 *
 * 更换渲染器引用的精灵，将重置缩放比率，按原图尺寸绘制。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_set_w(struct k_sprite_renderer *renderer, float scaled_w);

/**
 * \brief 设置渲染器水平方向的缩放比率
 *
 * 渲染器将延原精灵图的水平按比率缩放精灵，变换原点为精灵的原点。
 *
 * - 若 `scale_x` 为 1.0f，则按原图尺寸绘制。
 * - 若 `scale_x` 为 1.0f，则拉伸精灵图宽度到原图的两倍。
 * - 若 `scale_x` 为 0.5f，则压缩精灵图宽度到原图的一半。
 * - 若 `scale_x` 为 0 或负值，则将其设置为 0，此时精灵图被压缩至不可见。
 *
 * 更换渲染器引用的精灵，将重置缩放比率，按原图尺寸绘制。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_scale_x(struct k_sprite_renderer *renderer, float scale_x);

/**
 * \brief 获取渲染器绘制精灵的宽度
 *
 * 函数返回渲染器绘制精灵的宽度。若渲染器没有引用精灵，则函数返回 0。
 */
float k_sprite_renderer_get_w(struct k_sprite_renderer *renderer);

/* endregion */

/* region [scale_y] */

/**
 * \brief 设置渲染器绘制精灵的高度
 *
 * 渲染器将延原精灵图的竖直方向缩放精灵，变换原点为精灵的原点。
 *
 * - 若 `scaled_h` 为正值，则精灵图高度将被拉伸或压缩至该值。
 * - 若 `scaled_h` 为 0 或负值，则将其设置为 0，此时精灵图被压缩至不可见。
 *
 * 更换渲染器引用的精灵，将重置缩放比率，按原图尺寸绘制。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_set_h(struct k_sprite_renderer *renderer, float scaled_h);

/**
 * \brief 设置渲染器竖直方向的缩放比率
 *
 * 渲染器将延原精灵图的竖直按比率缩放精灵，变换原点为精灵的原点。
 *
 * - 若 `scale_y` 为 1.0f，则按原图尺寸绘制。
 * - 若 `scale_y` 为 1.0f，则拉伸精灵图高度到原图的两倍。
 * - 若 `scale_y` 为 0.5f，则压缩精灵图高度到原图的一半。
 * - 若 `scale_y` 为 0 或负值，则将其设置为 0，此时精灵图被压缩至不可见。
 *
 * 更换渲染器引用的精灵，将重置缩放比率，按原图尺寸绘制。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_scale_y(struct k_sprite_renderer *renderer, float scale_y);

/**
 * \brief 获取渲染器绘制精灵的高度
 *
 * 函数返回渲染器绘制精灵的高度。若渲染器没有引用精灵，则函数返回 0。
 */
float k_sprite_renderer_get_h(struct k_sprite_renderer *renderer);

/* endregion */

/* region [rotate] */

/**
 * \brief 设置渲染器绘制精灵的旋转角度
 *
 * 渲染器将旋转绘制精灵，变换原点为精灵的原点。
 * 旋转单位采用角度制，顺时针方向为正方向。
 *
 * 渲染器内部不会将角度限制在 0 到 360 度范围内，
 * 是否需要对角度值进行取模运算以限制其范围，取决于你。
 *
 * 更换渲染器引用的精灵将重置旋转角度为 0。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_rotate(struct k_sprite_renderer *renderer, float angle);

/**
 * \brief 获取渲染器绘制精灵的旋转角度
 *
 * 函数返回渲染器绘制精灵的的旋转角度。若渲染器没有引用精灵，则函数返回 0。
 */
float k_sprite_renderer_get_rotation(struct k_sprite_renderer *renderer);

/* endregion */

/* region [flip_x] */

/**
 * \brief 设置渲染器延水平方向翻转绘制精灵
 *
 * 若 `flip` 为非 0，则启用水平翻转，否则不启用。
 * 变换原点为精灵的原点。
 *
 * 更换渲染器引用的精灵，将取消翻转。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_flip_x(struct k_sprite_renderer *renderer, int flip);

/**
 * \brief 检查渲染器是否启用水平翻转
 *
 * 若渲染器启用水平翻转，则函数返回非 0，否则返回 0。
 * 若渲染器没有引用精灵，则函数返回 0。
 */
int k_sprite_renderer_is_flipped_x(struct k_sprite_renderer *renderer);

/* endregion */

/* region [flip_y] */

/**
 * \brief 设置渲染器延竖直方向翻转绘制精灵
 *
 * 若 `flip` 为非 0，则启用竖直翻转，否则不启用。
 * 变换原点为精灵的原点。
 *
 * 更换渲染器引用的精灵，将取消翻转。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_flip_y(struct k_sprite_renderer *renderer, int flip);

/**
 * \brief 检查渲染器是否启用竖直翻转
 *
 * 若渲染器启用竖直翻转，则函数返回非 0，否则返回 0。
 * 若渲染器没有引用精灵，则函数返回 0。
 */
int k_sprite_renderer_is_flipped_y(struct k_sprite_renderer *renderer);

/* endregion */

/**
 * \brief 重置渲染器对精灵图的所有变换操作
 *
 * 重置宽高缩放倍率为 1.0f，旋转角度为 0，取消水平翻转与垂直翻转。
 * 即按精灵图的原样绘制，不缩放、不旋转、不翻转。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 * 更改渲染器引用的精灵时，也会重置所有的变换。
 */
void k_sprite_renderer_reset_transforms(struct k_sprite_renderer *renderer);

/* endregion */

/* region [sprite_renderer set/get] */

/* region [z_index] */

/**
 * \brief 更改渲染器的绘制深度
 *
 * 即使渲染器没有引用精灵，也可以修改绘制深度。
 * 更换渲染器引用的精灵，不会改变渲染器的绘制深度。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_renderer_set_z_index(struct k_sprite_renderer *renderer, int z_index);

/** \brief 获取渲染器的绘制深度 */
int k_sprite_renderer_get_z_index(struct k_sprite_renderer *renderer);

/* endregion */

/* region [speed] */

/**
 * \brief 设置渲染器播放精灵动画的速度
 *
 * - 若 `speed` 为 1.0f，即按照精灵每帧定义的 `delay` 值原速播放。
 * - 若 `speed` 为 2.0f，则 2 倍速快速播放。
 * - 若 `speed` 为 0.5f，则 0.5 倍速慢速播放。
 * - 若 `speed` 为 0 或负值，则将其设置为 0，并暂停播放。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 * 更换渲染器引用的精灵，将重置播放倍速为 1.0f。
 */
void k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed);

/**
 * \brief 获取渲染器播放精灵动画的速度
 *
 * 函数返回渲染器播放精灵动画的速度。若渲染器没有引用精灵，则函数返回 0。
 */
float k_sprite_renderer_get_speed(struct k_sprite_renderer *renderer);

/* endregion */

/* region [loop] */

/**
 * \brief 设置渲染器播放精灵动画的次数
 *
 * 渲染器将循环播放精灵动画 `loop` 次，然后停在最后一帧。
 *
 * 渲染器不支持无限播放，但指定 `loop` 设为 `INT_MAX`，也能达到“无限”的效果。
 * 播放次数最少为 1，若指定 `loop` 为 0 或负值，渲染器仍会播放 1 次动画。
 *
 * 更换渲染器引用的精灵，将重置播放次数为 `INT_MAX`。
 */
void k_sprite_renderer_set_loop(struct k_sprite_renderer *renderer, int loop);

/**
 * \brief 设置渲染器每播放完一次动画后执行的回调函数
 *
 * 渲染器每播放完一次动画，便会执行一次该回调。
 *
 * 更换渲染器引用的精灵，将取消回调。
 */
void k_sprite_renderer_set_loop_callback(struct k_sprite_renderer *renderer, void (*fn_callback)(void *data), void *data);

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
 * 函数返回渲染器当前引用的精灵，若没有则返回 `NULL`。
 */
struct k_sprite *k_sprite_renderer_get_sprite(struct k_sprite_renderer *renderer);

/* endregion */

/* endregion */

/* endregion */

/* region [sprite_renderer debug] */

/**
 * \brief 开启或关闭渲染器调试模式
 *
 * 若 `debug` 为非 0 则开启调试模式，否则关闭。
 *
 * 调试模式下，渲染器将用绿色绘制精灵图的矩形边框，并在精灵原点处绘制十字。
 * 绘制图层深度为 `K_SPRITE_RENDERER_DEBUG_DRAW_LAYER`。
 *
 * 切换渲染器引用的精灵不会关闭调试模式。
 * 即使渲染器没有引用精灵，也可以修改开启或关闭调试模式。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_renderer_set_debug(struct k_sprite_renderer *renderer, int debug);

/** \brief 精灵渲染器调试模式绘制图层深度 */
#define K_SPRITE_RENDERER_DEBUG_DRAW_LAYER 100000

/* endregion */

#endif
