#include <assert.h>

#include "k_game/core/k_time.h"
#include "k_game/core/k_canvas.h"

#include "./k_sprite_renderer_internal.h"

/* region [draw_sprite] */

static void k__sprite_renderer_on_draw(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);
    renderer->timer += k_time_get_step_delta() * renderer->speed; /* 获取帧间隔的事件，同时乘以一个倍率，可以实现倍速播放 */
    void (*fn_loop_callback)(struct k_object *object) = NULL;
    struct k_sprite *sprite = renderer->sprite;
    size_t frame_idx_last = k_sprite_get_frames_num(sprite) - 1;
    while (1) { /* 精灵渲染器内部的计时器累加时间，算出播放进度应该是到达哪一帧 */
        float delay = (float)k_sprite_get_frame_delay(sprite, renderer->frame_idx) / 1000.0f;
        if (renderer->timer < delay) break;
        renderer->timer -= delay;
        if (renderer->frame_idx < frame_idx_last) {
            renderer->frame_idx += 1;
        } else {
            if (2 <= renderer->loop_count) { /* loop_count 记录渲染器重复播放动画的次数（有的动画只播放一次，例如角色死亡的动画） */
                fn_loop_callback = renderer->fn_loop_callback;
                renderer->loop_count -= 1;
                renderer->frame_idx = 0;
            } else if (1 == renderer->loop_count) {
                fn_loop_callback = renderer->fn_loop_callback;
                renderer->loop_count -= 1;
                renderer->frame_idx = frame_idx_last;
            } else {
                renderer->frame_idx = frame_idx_last; /* 若该精灵 */
            }
        }
    }
    if (transform_none == renderer->transform_flags) { /* 绘制精灵帧。若渲染器没有应用缩放、旋转等变换，则使用简单绘制 */
        k_canvas_room_draw_sprite(sprite, renderer->frame_idx, *(renderer->x), *(renderer->y), NULL);
    } else {
        struct k_canvas_draw_sprite_options opts;
        opts.scaled_w = renderer->scaled_w;
        opts.scaled_h = renderer->scaled_h;
        opts.angle    = renderer->angle;
        opts.flip_x   = renderer->transform_flags & transform_flip_x;
        opts.flip_y   = renderer->transform_flags & transform_flip_y;
        k_canvas_room_draw_sprite(sprite, renderer->frame_idx, *(renderer->x), *(renderer->y), &opts);
    } /* 否则启用复杂绘制 */
    if (NULL != fn_loop_callback) { /* 执行回调，告知用户已播放完一轮精灵动画 */
        fn_loop_callback(k_component_get_object(component));
    }
}

/* endregion */

/* region [ref_sprite] */

int k_sprite_renderer_set_sprite(struct k_sprite_renderer *renderer, struct k_sprite *sprite) {

    if (NULL == sprite) {

        if (NULL == renderer->sprite)
            return 0;

        assert(NULL != renderer->cb_draw_sprite);

        k_callback_del(renderer->cb_draw_sprite);
        renderer->cb_draw_sprite = NULL;

        renderer->sprite = NULL;
        return 0;
    }
    else {
        if (NULL == renderer->sprite) {

            assert(NULL == renderer->cb_draw_sprite);

            renderer->cb_draw_sprite = k_component_add_draw_callback(
                renderer->component,
                k__sprite_renderer_on_draw,
                renderer->z_group,
                renderer->z_layer
            );
            if (NULL == renderer->cb_draw_sprite)
                return -1;
        }

        renderer->sprite = sprite;
        k__sprite_renderer_reset(renderer);
        return 0;
    }
}

struct k_sprite *k_sprite_renderer_get_sprite(struct k_sprite_renderer *renderer) {
    return renderer->sprite;
}

/* endregion */

/* region [z_index] */

int k_sprite_renderer_set_z_index(struct k_sprite_renderer *renderer, int z_group, int z_layer) {

    if (z_group == renderer->z_group && z_layer == renderer->z_layer)
        return 0;

    if (NULL != renderer->sprite) {
        struct k_callback *callback = k_component_add_draw_callback(renderer->component, k__sprite_renderer_on_draw, z_group, z_layer);
        if (NULL == callback)
            return -1;

        k_callback_del(renderer->cb_draw_sprite);
        renderer->cb_draw_sprite = callback;
    }

    renderer->z_group = z_group;
    renderer->z_layer = z_layer;
    return 0;
}

int k_sprite_renderer_set_z_group(struct k_sprite_renderer *renderer, int z_group) {
    return k_sprite_renderer_set_z_index(renderer, z_group, renderer->z_layer);
}

int k_sprite_renderer_set_z_layer(struct k_sprite_renderer *renderer, int z_layer) {
    return k_sprite_renderer_set_z_index(renderer, renderer->z_group, z_layer);
}

int k_sprite_renderer_get_z_group(struct k_sprite_renderer *renderer) {
    return renderer->z_group;
}

int k_sprite_renderer_get_z_layer(struct k_sprite_renderer *renderer) {
    return renderer->z_layer;
}

/* endregion */

/* region [speed] */

void k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed) {

    if (speed <= 0.0f)
        renderer->speed = 0.0f;
    else
        renderer->speed = speed;
}

void k_sprite_renderer_set_duration(struct k_sprite_renderer *renderer, float duration) {

    if (NULL == renderer->sprite || duration <= 0.0f)
        return;

    int sum_delay = 0;

    size_t i = 0;
    size_t frames_num = k_sprite_get_frames_num(renderer->sprite);
    for (; i < frames_num; i++) {
        sum_delay += k_sprite_get_frame_delay(renderer->sprite, i);
    }

    renderer->speed = ((float)(sum_delay) / 1000.0f) / duration;
}

float k_sprite_renderer_get_speed(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0;
    else
        return renderer->speed;
}

/* endregion */

/* region [loop_count] */

void k_sprite_renderer_set_loop_count(struct k_sprite_renderer *renderer, int loop_count) {

    if (loop_count <= 1)
        renderer->loop_count = 1;
    else
        renderer->loop_count = loop_count;
}

int k_sprite_renderer_get_loop_count(struct k_sprite_renderer *renderer) {
    return renderer->loop_count;
}

void k_sprite_renderer_set_loop_callback(struct k_sprite_renderer *renderer, void (*fn_callback)(struct k_object *object)) {
    renderer->fn_loop_callback = fn_callback;
}

/* endregion */
