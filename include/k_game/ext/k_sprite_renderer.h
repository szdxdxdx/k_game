#ifndef K_GAME_COMPONENT_SPRITE_RENDERER_H
#define K_GAME_COMPONENT_SPRITE_RENDERER_H

struct k_sprite;

struct k_sprite_renderer_config {

    float *x;
    float *y;

    struct k_sprite *sprite;

    int z_index;
};

struct k_sprite_renderer;

/**
 * \brief 为对象添加一个精灵渲染器组件
 *
 * 若成功，函数返回组件指针，否则返回 `NULL`。
 */
struct k_sprite_renderer *k_object_add_sprite_renderer(struct k_object *object, const struct k_sprite_renderer_config *config);

/**
 * \brief 移除对象上挂载的精灵渲染器组件
 *
 * 若 `renderer` 为 `NULL`，则函数不做任何事。
 */
void k_object_del_sprite_renderer(struct k_sprite_renderer *renderer);

/**
 * \brief 设置渲染器引用的精灵
 *
 * 绑定新精灵后，渲染器会重置所有状态（精灵帧播放速度、图像缩放、旋转、翻转等）。
 * 若指定新的精灵为 `NULL`，则仅移除当前引用的精灵。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_sprite_renderer_set_sprite(struct k_sprite_renderer *renderer, struct k_sprite *sprite);

/**
 * \brief 获取渲染器当前引用的精灵
 */
struct k_sprite *k_sprite_renderer_get_sprite(struct k_sprite_renderer *renderer);

/**
 * \brief 设置渲染器播放精灵动画的速度
 *
 * 若渲染器没有引用精灵，或指定的速度为负值，则函数不做任何事。
 */
void k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed);

/**
 * \brief 增加或减少渲染器播放精灵动画的速度
 *
 * 若指定速度增量为正值，则加速播放，若为负值则减速播放。
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

/* region [transform] */

/**
 * \brief 设置渲染器绘制精灵的宽度
 *
 * 指定的宽度值不能为负，
 * 渲染器将精灵延水平方向的尺寸拉伸或压缩至该值。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_set_scale_w(struct k_sprite_renderer *renderer, int scaled_w);

/**
 * \brief 增加或减少渲染器绘制精灵的宽度
 *
 * 若指定的宽度增量为正，则拉伸精灵图，若为负值则压缩精灵图。
 * 宽度最小为 0，此时精灵图被压缩到看不见。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_add_scale_w(struct k_sprite_renderer *renderer, int w_add);

/**
 * \brief 获取渲染器绘制精灵的宽度
 *
 * 若渲染器没有引用精灵，则函数返回宽度值为 0。
 */
int k_sprite_renderer_get_scale_w(struct k_sprite_renderer *renderer);

/**
 * \brief 设置渲染器绘制精灵的高度
 *
 * 指定的高度值不能为负，
 * 渲染器将精灵延竖直方向的尺寸拉伸或压缩至该值。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_set_scale_h(struct k_sprite_renderer *renderer, int scaled_h);

/**
 * \brief 增加或减少渲染器绘制精灵的高度
 *
 * 若指定的宽度增量为正，则拉伸精灵图，若为负值则压缩精灵图。
 * 高度最小为 0，此时精灵图被压缩到看不见。
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_add_scale_h(struct k_sprite_renderer *renderer, int h_add);

/**
 * \brief 获取渲染器绘制精灵的高度
 *
 * 若渲染器没有引用精灵，则函数返回高度值为 0。
 */
int k_sprite_renderer_get_scale_h(struct k_sprite_renderer *renderer);

void k_sprite_renderer_set_rotation(struct k_sprite_renderer *renderer, float angle);

void k_sprite_renderer_add_rotation(struct k_sprite_renderer *renderer, float angle_add);

float k_sprite_renderer_get_rotation(struct k_sprite_renderer *renderer);

void k_sprite_renderer_flip_x(struct k_sprite_renderer *renderer);

int k_sprite_renderer_is_flipped_x(struct k_sprite_renderer *renderer);

void k_sprite_renderer_flip_y(struct k_sprite_renderer *renderer);

int k_sprite_renderer_is_flipped_y(struct k_sprite_renderer *renderer);

/**
 * \brief 重置渲染器对图像的所有变换操作（缩放、旋转和翻转）
 *
 * 若渲染器没有引用精灵，则函数不做任何事。
 */
void k_sprite_renderer_clear_transforms(struct k_sprite_renderer *renderer);

/* endregion */

#endif
