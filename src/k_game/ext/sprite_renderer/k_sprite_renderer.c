#include "k_game.h"

#include "../k_components_def.h"

enum renderer_transform {
        transform_none      = 0b00000,
        transform_scaled_w  = 0b00001,
        transform_scaled_h  = 0b00010,
        transform_rotation  = 0b00100,
        transform_flip_x    = 0b01000,
        transform_flip_y    = 0b10000,
    };

struct k_sprite_renderer {
    struct k_component *component;
    struct k_component_callback *draw_callback;

    struct k_sprite *sprite;

    float timer;

    float speed;

    size_t frame_idx;

    float *x;
    float *y;

    /* 伸缩变换 */
    int scaled_w;
    int scaled_h;

    /* 旋转变换 */
    float angle;

    /* 用于指示在绘制操作中需要应用哪些变换
     *
     * 每一个比特位对应一种变换，
     * 由枚举 `enum renderer_transform` 定义。
     */
    uint8_t transform_flags;
};

static void renderer_draw(struct k_component *component);

/* region [sprite_renderer] */

static int sprite_renderer_init(struct k_component *component, void *params) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);
    struct k_sprite_renderer_config *config = params;

    struct k_component_callback *draw_callback = k_component_add_draw_callback(component, renderer_draw, config->z_index);
    if (NULL == draw_callback)
        return -1;

    renderer->component     = component;
    renderer->draw_callback = draw_callback;

    renderer->sprite    = config->sprite;
    renderer->timer     = 0;
    renderer->frame_idx = 0;
    renderer->speed     = 1.0f;

    renderer->x = config->x;
    renderer->y = config->y;

    renderer->transform_flags = transform_none;

    renderer->scaled_w = k_sprite_get_width(config->sprite);
    renderer->scaled_h = k_sprite_get_height(config->sprite);
    renderer->angle    = 0.0f;

    return 0;
}

void k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed) {
    if (0.0f <= speed)
        renderer->speed = speed;
}

void k_sprite_renderer_add_speed(struct k_sprite_renderer *renderer, float speed_add) {
    k_sprite_renderer_set_speed(renderer, renderer->speed + speed_add);
}

void k_sprite_renderer_set_sprite(struct k_sprite_renderer *renderer, struct k_sprite *sprite) {
    renderer->timer     = 0;
    renderer->frame_idx = 0;
    renderer->sprite    = sprite;
}

void k_sprite_renderer_set_scale_w(struct k_sprite_renderer *renderer, int scaled_w) {

    if (scaled_w <= 0)
        scaled_w = 0;

    renderer->scaled_w = scaled_w;

    if (k_sprite_get_width(renderer->sprite) != scaled_w)
        renderer->transform_flags |= transform_scaled_w;
    else
        renderer->transform_flags &= ~transform_scaled_w;
}

void k_sprite_renderer_add_scale_w(struct k_sprite_renderer *renderer, int w_add) {
    k_sprite_renderer_set_scale_w(renderer, renderer->scaled_w + w_add);
}

void k_sprite_renderer_set_scale_h(struct k_sprite_renderer *renderer, int scaled_h) {

    if (scaled_h <= 0)
        scaled_h = 0;

    renderer->scaled_h = scaled_h;

    if (k_sprite_get_height(renderer->sprite) != scaled_h)
        renderer->transform_flags |= transform_scaled_h;
    else
        renderer->transform_flags &= ~transform_scaled_h;
}

void k_sprite_renderer_add_scale_h(struct k_sprite_renderer *renderer, int h_add) {
    k_sprite_renderer_set_scale_h(renderer, renderer->scaled_h + h_add);
}

void k_sprite_renderer_set_rotation(struct k_sprite_renderer *renderer, float angle) {

    renderer->angle = angle;

    if (angle <= -0.0001f || 0.0001f <= angle)
        renderer->transform_flags |= transform_rotation;
    else
        renderer->transform_flags &= ~transform_rotation;
}

void k_sprite_renderer_add_rotation(struct k_sprite_renderer *renderer, float angle_add) {
    k_sprite_renderer_set_rotation(renderer, renderer->angle + angle_add);
}

void k_sprite_renderer_flip_x(struct k_sprite_renderer *renderer) {
    renderer->transform_flags ^= transform_flip_x;
}

void k_sprite_renderer_flip_y(struct k_sprite_renderer *renderer) {
    renderer->transform_flags ^= transform_flip_y;
}

void k_sprite_renderer_clear_transforms(struct k_sprite_renderer *renderer) {
    renderer->transform_flags = transform_none;

    renderer->scaled_w = k_sprite_get_width(renderer->sprite);
    renderer->scaled_h = k_sprite_get_height(renderer->sprite);
    renderer->angle    = 0.0f;
}

/* endregion */

/* region [component_callback] */

int k_sprite_renderer_set_z_index(struct k_sprite_renderer *renderer, int z_index) {

    struct k_component_callback *draw_callback = k_component_add_draw_callback(renderer->component, renderer_draw, z_index);
    if (NULL == draw_callback)
        return -1;

    k_component_del_callback(renderer->draw_callback);
    renderer->draw_callback = draw_callback;
    return 0;
}

static inline void calc_current_frame(struct k_sprite_renderer *renderer) {

    struct k_sprite *sprite = renderer->sprite;
    size_t frames_num = k_sprite_get_frames_num(sprite);

    renderer->timer += k_get_step_delta() * renderer->speed;

    uint64_t timer_ms = (uint64_t)(renderer->timer * 1000);
    while (1) {
        int delay = k_sprite_get_frame_delay(sprite, renderer->frame_idx);
        if (timer_ms < delay)
            break;

        timer_ms -= delay;
        renderer->frame_idx += 1;
        renderer->frame_idx %= frames_num;
    }

    renderer->timer = (float)timer_ms / 1000.0f;
}

static inline void renderer_draw_sprite(struct k_sprite_renderer *renderer) {

    if ( ! renderer->transform_flags) {
        k_sprite_draw(renderer->sprite, renderer->frame_idx, *(renderer->x), *(renderer->y));
    }
    else {
        struct k_sprite_draw_options opt;
        opt.x               = *(renderer->x);
        opt.y               = *(renderer->y);
        opt.dst_w           = renderer->scaled_w;
        opt.dst_h           = renderer->scaled_h;
        opt.angle           = renderer->angle;
        opt.horizontal_flip = renderer->transform_flags & transform_flip_x;
        opt.vertical_flip   = renderer->transform_flags & transform_flip_y;

        k_sprite_draw_EX(renderer->sprite, renderer->frame_idx, &opt);
    }
}

static void renderer_draw(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);
    calc_current_frame(renderer);
    renderer_draw_sprite(renderer);
}

/* endregion */

/* region [component_define && object_add_component] */

static struct k_component_type *spr_rdr = NULL;

int k__component_def_sprite_renderer(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size = sizeof(struct k_sprite_renderer);
    config.fn_init = sprite_renderer_init;

    spr_rdr = k_component_define(NULL, &config);
    if (NULL == spr_rdr)
        return -1;

    k_component_type_set_name(spr_rdr, "k/spr-rdr");
    return 0;
}

struct k_sprite_renderer *k_object_add_sprite_renderer(struct k_object *object, const struct k_sprite_renderer_config *config) {

    if (NULL == object)
        return NULL;

    struct k_component *component = k_object_add_component(object, spr_rdr, (void*)config);
    if (NULL == component)
        return NULL;
    else
        return k_component_get_data(component);
}

void k_object_del_sprite_renderer(struct k_sprite_renderer *renderer) {
    if (NULL != renderer)
        k_object_del_component(renderer->component);
}

/* endregion */
