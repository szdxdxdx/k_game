#ifndef K_GAME_COMPONENT_SPRITE_RENDERER_H
#define K_GAME_COMPONENT_SPRITE_RENDERER_H

struct k_sprite;

/**
 * \brief 精灵渲染器
 *
 * 精灵渲染器的本质是一个组件。
 *
 * 一般来说，对象都至少有一个关联的精灵作为其外观表现。
 * 给对象挂载上精灵渲染器组件，并指定其引用的精灵图，
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
     * 指定渲染器初始化时引用的精灵。
     * 若为 `NULL`，则不引用任何精灵。
     *
     * 渲染器仅持有精灵的指针，不复制精灵数据，不管理精灵的生命周期。
     * 若要销毁精灵，请确保该精灵未被任何渲染器引用，以避免悬空指针。
     */
    struct k_sprite *sprite;

    /** \brief 指定渲染器绘制精灵的深度 */
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

/* region [sprite_renderer set/get] */

/**
 * \brief 设置渲染器引用的精灵
 *
 * 引用新精灵后，渲染器会重置计时器，并定位到精灵的第 0 帧，
 * 但是不会重置播放速度，以及不会重置图像缩放、旋转、翻转等变换效果。
 *
 * 若指定新的精灵为 `NULL`，则仅移除当前引用的精灵。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_renderer_set_sprite(struct k_sprite_renderer *renderer, struct k_sprite *sprite);

/**
 * \brief 获取渲染器当前引用的精灵
 *
 * 函数返回渲染器当前引用的精灵。
 * 若渲染器没有引用精灵，则函数返回 `NULL`。
 */
struct k_sprite *k_sprite_renderer_get_sprite(struct k_sprite_renderer *renderer);

/**
 * \brief 设置渲染器播放精灵动画的速度
 *
 * 渲染器默认的播放速度值为 1.0f，即按照精灵每帧定义的 `delay` 值原速播放。
 * 若将渲染器的播放速度设为 2.0f，则 2 倍速播放精灵动画。
 *
 * 若渲染器没有引用精灵，或指定的速度为负值，则函数不做任何事。
 */
void k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed);

/**
 * \brief 增加或减少渲染器播放精灵动画的速度
 *
 * - 若 `speed_add` 为正值，则加速播放。
 * - 若 `speed_add` 为负值，则减速播放。
 * 速度最慢降至 0，此时暂停播放精灵动画。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_add_speed(struct k_sprite_renderer *renderer, float speed_add);

/**
 * \brief 获取渲染器播放精灵动画的速度
 *
 * 若渲染器没有引用精灵，则函数返回速度值为 0.0f。
 */
float k_sprite_renderer_get_speed(struct k_sprite_renderer *renderer);

/**
 * \brief 更改渲染器的绘制深度
 *
 * 若渲染器没有引用精灵，则更改失败。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_renderer_set_z_index(struct k_sprite_renderer *renderer, int z_index);

/* endregion */

/* region [sprite_renderer transform] */

/**
 * \brief 设置渲染器绘制精灵的宽度
 *
 * 渲染器将延原精灵图的水平方向缩放精灵，变换原点为精灵的中心。
 *
 * - 若 `scaled_w` 为正值，则精灵图宽度将被拉伸或压缩至该值。
 * - 若 `scaled_w` 为 0 或负值，则精灵图被压缩至不可见。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_set_w(struct k_sprite_renderer *renderer, int scaled_w);

/**
 * \brief 调整渲染器绘制精灵的宽度
 *
 * 渲染器将延原精灵图的水平方向缩放精灵，变换原点为精灵的中心。
 *
 * - 若 `w_delta` 为正值，则增加宽度，拉伸精灵图。
 * - 若 `w_delta` 为负值，则减少宽度，压缩精灵图。
 *
 * 宽度最小减少到 0，此时精灵图被压缩至不可见。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_adjust_w(struct k_sprite_renderer *renderer, int w_delta);

/**
 * \brief 设置渲染器水平方向的缩放比率
 *
 * 渲染器将延原精灵图的水平按比率缩放精灵，变换原点为精灵的中心。
 *
 * 缩放比率 `scale_x` 必须为正值。
 * - 若 `scale_x` 为 1.0f，则按原尺寸绘制。
 * - 若 `scale_x` 为 1.0f，则拉伸精灵图宽度到原来的两倍。
 * - 若 `scale_x` 为 0.5f，则压缩精灵图宽度到原来的一半。
 * - 若 `scale_x` 为 0.0f 或负值，则精灵图被压缩至不可见。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_scale_x(struct k_sprite_renderer *renderer, float scale_x);

/**
 * \brief 获取渲染器绘制精灵的宽度
 *
 * 若渲染器没有引用精灵，则函数返回宽度值为 0。
 */
int k_sprite_renderer_get_w(struct k_sprite_renderer *renderer);

/**
 * \brief 设置渲染器绘制精灵的高度
 *
 * 渲染器将延原精灵图的竖直方向缩放精灵，变换原点为精灵的中心。
 *
 * - 若 `scaled_h` 为正值，则精灵图高度将被拉伸或压缩至该值。
 * - 若 `scaled_h` 为 0 或负值，则精灵图被压缩至不可见。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_set_h(struct k_sprite_renderer *renderer, int scaled_h);

/**
 * \brief 调整渲染器绘制精灵的高度
 *
 * 渲染器将延原精灵图的竖直方向缩放精灵，变换原点为精灵的中心。
 *
 * - 若 `h_delta` 为正值，则增加高度，拉伸精灵图。
 * - 若 `h_delta` 为负值，则减少高度，压缩精灵图。
 *
 * 高度最小减少到 0，此时精灵图被压缩至不可见。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_adjust_h(struct k_sprite_renderer *renderer, int h_delta);

/**
 * \brief 设置渲染器竖直方向的缩放比率
 *
 * 渲染器将延原精灵图的竖直按比率缩放精灵，变换原点为精灵的中心。
 *
 * 缩放比率 `scale_y` 必须为正值。
 * - 若 `scale_y` 为 1.0f，则按原尺寸绘制。
 * - 若 `scale_y` 大于 1.0f，则该按比率拉伸精灵图高度。
 * - 若 `scale_y` 小于 1.0f，则该按比率压缩精灵图高度。
 * - 若 `scale_y` 为 0.0f 或负值，则精灵图被压缩至不可见。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_scale_y(struct k_sprite_renderer *renderer, float scale_y);

/**
 * \brief 获取渲染器绘制精灵的高度
 *
 * 若渲染器没有引用精灵，则函数返回高度值为 0。
 */
int k_sprite_renderer_get_h(struct k_sprite_renderer *renderer);

/**
 * \brief 设置渲染器绘制精灵的旋转角度
 *
 * 渲染器将旋转绘制精灵，变换原点为精灵的中心。
 * 旋转单位采用角度制，顺时针方向为正方向。
 *
 * 渲染器内部不会将角度限制在 0 到 360 度范围内，
 * 是否需要对角度值进行取模运算以限制其范围，取决于你。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_rotate(struct k_sprite_renderer *renderer, float angle);

/**
 * \brief 增加或减少渲染器绘制精灵的旋转角度
 *
 * 渲染器将旋转绘制精灵，变换原点为精灵的中心。
 * 旋转单位采用角度制，顺时针方向为正方向。
 *
 * - 若 `angle_delta` 为正值，则顺时针旋转精灵图，
 * - 若 `angle_delta` 为负值，则逆时针旋转精灵图。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_adjust_rotation(struct k_sprite_renderer *renderer, float angle_delta);

/**
 * \brief 获取渲染器绘制精灵的旋转角度
 *
 * 若渲染器没有引用精灵，则函数返回角度值为 0.0f。
 */
float k_sprite_renderer_get_rotation(struct k_sprite_renderer *renderer);

/**
 * \brief 设置渲染器延水平方向翻转绘制精灵
 *
 * 若 `flip` 为非 0，则启用水平翻转，否则禁用水平翻转。
 * 变换原点为精灵的中心。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_flip_x(struct k_sprite_renderer *renderer, int flip);

/**
 * \brief 检查渲染器是否启用水平翻转
 *
 * 若渲染器启用水平翻转，则函数返回非 0，否则返回 0。
 *
 * 若渲染器没有引用精灵，则函数返回 0。
 */
int k_sprite_renderer_is_flipped_x(struct k_sprite_renderer *renderer);

/**
 * \brief 设置渲染器延竖直方向翻转绘制精灵
 *
 * 若 `flip` 为非 0，则启用竖直翻转，否则禁用竖直翻转。
 * 变换原点为精灵的中心。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_flip_y(struct k_sprite_renderer *renderer, int flip);

/**
 * \brief 检查渲染器是否启用竖直翻转
 *
 * 若渲染器启用竖直翻转，则函数返回非 0，否则返回 0。
 *
 * 若渲染器没有引用精灵，则函数返回 0。
 */
int k_sprite_renderer_is_flipped_y(struct k_sprite_renderer *renderer);

/**
 * \brief 重置渲染器对图像的所有变换操作
 *
 * 重置宽高缩放倍率为 1.0f，旋转角度为 0.0f，禁用水平翻转与垂直翻转。
 * 即按精灵图的原始尺寸绘制，不旋转、不翻转。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_clear_transforms(struct k_sprite_renderer *renderer);

/* endregion */

#endif
