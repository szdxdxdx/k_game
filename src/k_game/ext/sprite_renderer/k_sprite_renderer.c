#include "k_game.h"

#include "../k_components_def.h"

/* 利用位标识来指示在绘制操作中需要应用哪些变换 */
enum renderer_transform {
    transform_none     = 0,
    transform_scale_x  = 1 << 0,
    transform_scale_y  = 1 << 1,
    transform_rotate   = 1 << 2,
    transform_flip_x   = 1 << 3,
    transform_flip_y   = 1 << 4,
};

struct k_sprite_renderer {
    struct k_component *component;

    struct k_component_callback *draw_callback;
    int z_index;

    struct k_sprite *sprite;
    float timer;
    float speed;
    size_t frame_idx;

    float *x;
    float *y;

    int scaled_w;
    int scaled_h;
    float angle;

    uint8_t transform_flags;
};

/* region [set_speed] */

void k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed) {

    if (NULL == renderer->sprite)
        return;

    if (speed <= 0.0f)
        renderer->speed = 0.0f;
    else
        renderer->speed = speed;
}

void k_sprite_renderer_add_speed(struct k_sprite_renderer *renderer, float speed_add) {
    k_sprite_renderer_set_speed(renderer, renderer->speed + speed_add);
}

float k_sprite_renderer_get_speed(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0.0f;
    else
        return renderer->speed;
}

/* endregion */

/* region [set_transform] */

/* region [scale] */

void k_sprite_renderer_set_w(struct k_sprite_renderer *renderer, int scaled_w) {

    if (NULL == renderer->sprite)
        return;

    if (scaled_w <= 0) {
        renderer->scaled_w = 0;
    } else {
        renderer->scaled_w = scaled_w;

        if (k_sprite_get_width(renderer->sprite) != scaled_w)
            renderer->transform_flags |= transform_scale_x;
        else
            renderer->transform_flags &= ~transform_scale_x;
    }
}

void k_sprite_renderer_adjust_w(struct k_sprite_renderer *renderer, int w_delta) {
    k_sprite_renderer_set_w(renderer, renderer->scaled_w + w_delta);
}

void k_sprite_renderer_scale_x(struct k_sprite_renderer *renderer, float scale_x) {

    if (NULL == renderer->sprite)
        return;

    if (scale_x <= 0.0f) {
        renderer->scaled_w = 0;
        renderer->transform_flags |= transform_scale_x;
    }
    else {
        int sprite_w = k_sprite_get_width(renderer->sprite);

        renderer->scaled_w = (int)((float)sprite_w * scale_x);

        if (sprite_w != renderer->scaled_w)
            renderer->transform_flags |= transform_scale_x;
        else
            renderer->transform_flags &= ~transform_scale_x;
    }
}

int k_sprite_renderer_get_w(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0;
    else
        return renderer->scaled_w;
}

/* endregion */

/* region [scale_y] */

void k_sprite_renderer_set_h(struct k_sprite_renderer *renderer, int scaled_h) {

    if (NULL == renderer->sprite)
        return;

    if (scaled_h <= 0) {
        renderer->scaled_h = 0;
    } else {
        renderer->scaled_h = scaled_h;

        if (k_sprite_get_height(renderer->sprite) != scaled_h)
            renderer->transform_flags |= transform_scale_y;
        else
            renderer->transform_flags &= ~transform_scale_y;
    }
}

int k_sprite_renderer_get_h(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0;
    else
        return renderer->scaled_h;
}

void k_sprite_renderer_adjust_h(struct k_sprite_renderer *renderer, int h_delta) {
    k_sprite_renderer_set_h(renderer, renderer->scaled_h + h_delta);
}

void k_sprite_renderer_scale_y(struct k_sprite_renderer *renderer, float scale_y) {

    if (NULL == renderer->sprite)
        return;

    if (scale_y <= 0.0f) {
        renderer->scaled_h = 0;
        renderer->transform_flags |= transform_scale_y;
    }
    else {
        int sprite_h = k_sprite_get_height(renderer->sprite);

        renderer->scaled_h = (int)((float)sprite_h * scale_y);

        if (sprite_h != renderer->scaled_h)
            renderer->transform_flags |= transform_scale_y;
        else
            renderer->transform_flags &= ~transform_scale_y;
    }
}

/* endregion */

/* region [rotate] */

void k_sprite_renderer_rotate(struct k_sprite_renderer *renderer, float angle) {

    if (NULL == renderer->sprite)
        return;

    renderer->angle = angle;

    if (0.0f != angle)
        renderer->transform_flags |= transform_rotate;
    else
        renderer->transform_flags &= ~transform_rotate;
}

void k_sprite_renderer_adjust_rotation(struct k_sprite_renderer *renderer, float angle_delta) {
    k_sprite_renderer_rotate(renderer, renderer->angle + angle_delta);
}

float k_sprite_renderer_get_rotation(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0.0f;
    else
        return renderer->angle;
}

/* endregion */

/* region [flip_x] */

void k_sprite_renderer_flip_x(struct k_sprite_renderer *renderer, int flip) {

    if (NULL == renderer->sprite)
        return;

    if (0 != flip)
        renderer->transform_flags |= transform_flip_x;
    else
        renderer->transform_flags &= ~transform_flip_x;
}

int k_sprite_renderer_is_flipped_x(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0;
    else
        return renderer->transform_flags & transform_flip_x;
}

/* endregion */

/* region [flip_y] */

void k_sprite_renderer_flip_y(struct k_sprite_renderer *renderer, int flip) {

    if (NULL == renderer->sprite)
        return;

    if (0 != flip)
        renderer->transform_flags |= transform_flip_y;
    else
        renderer->transform_flags &= ~transform_flip_y;
}

int k_sprite_renderer_is_flipped_y(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0;
    else
        return renderer->transform_flags & transform_flip_y;
}

/* endregion */

void k_sprite_renderer_clear_transforms(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite) {
        renderer->scaled_w = 0;
        renderer->scaled_h = 0;
    } else {
        renderer->scaled_w = k_sprite_get_width(renderer->sprite);
        renderer->scaled_h = k_sprite_get_height(renderer->sprite);
    }

    renderer->angle = 0.0f;
    renderer->transform_flags = transform_none;
}

/* endregion */

/* region [renderer_callback] */

static inline void calc_frame_idx(struct k_sprite_renderer *renderer) {
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
        if (frames_num <= renderer->frame_idx)
            renderer->frame_idx = 0;
    }

    renderer->timer = (float)timer_ms / 1000.0f;
}

static inline void draw_sprite(struct k_sprite_renderer *renderer) {

    if ( ! renderer->transform_flags) {
        k_sprite_draw(renderer->sprite, renderer->frame_idx, *(renderer->x), *(renderer->y), NULL);
    }
    else {
        struct k_sprite_draw_options opts;
        opts.scaled_w = renderer->scaled_w;
        opts.scaled_h = renderer->scaled_h;
        opts.angle    = renderer->angle;
        opts.flip_x   = renderer->transform_flags & transform_flip_x;
        opts.flip_y   = renderer->transform_flags & transform_flip_y;

        k_sprite_draw(renderer->sprite, renderer->frame_idx, *(renderer->x), *(renderer->y), &opts);
    }
}

static void renderer_draw_animation(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);

    calc_frame_idx(renderer);
    draw_sprite(renderer);
}

static void renderer_draw_image(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);

    draw_sprite(renderer);
}

int k_sprite_renderer_set_z_index(struct k_sprite_renderer *renderer, int z_index) {

    if (NULL == renderer->sprite)
        return -1;

    /* TODO 待 draw_callback 支持直接修改 z_index 后，不再采取这种“一删一加”的做法 */

    struct k_component_callback *draw_callback;
    if (k_sprite_get_frames_num(renderer->sprite) == 1) {
        draw_callback = k_component_add_draw_callback(renderer->component, renderer_draw_image, z_index);
    } else {
        draw_callback = k_component_add_draw_callback(renderer->component, renderer_draw_animation, z_index);
    }

    if (NULL == draw_callback)
        return -1;

    k_component_del_callback(renderer->draw_callback);
    renderer->draw_callback = draw_callback;

    return 0;
}

/* endregion */

/* region [set_sprite] */

int k_sprite_renderer_set_sprite(struct k_sprite_renderer *renderer, struct k_sprite *sprite) {

    if (NULL == renderer->sprite) {
        if (NULL == sprite) {
            return 0;
        }

        struct k_component_callback *draw_callback;
        if (k_sprite_get_frames_num(sprite) == 1)
            draw_callback = k_component_add_draw_callback(renderer->component, renderer_draw_image, renderer->z_index);
        else
            draw_callback = k_component_add_draw_callback(renderer->component, renderer_draw_animation, renderer->z_index);

        if (NULL == draw_callback)
            return -1;

        renderer->draw_callback = draw_callback;

        renderer->sprite    = sprite;
        renderer->timer     = 0.0f;
        renderer->speed     = 1.0f;
        renderer->frame_idx = 0;

        renderer->scaled_w = k_sprite_get_width(renderer->sprite);
        renderer->scaled_h = k_sprite_get_height(renderer->sprite);
        renderer->angle    = 0.0f;

        renderer->transform_flags = transform_none;
    }
    else {
        if (NULL == sprite) {
            k_component_del_callback(renderer->draw_callback);
            renderer->draw_callback = NULL;
            renderer->sprite = NULL;
            return 0;
        }

        struct k_component_callback *draw_callback;
        if (k_sprite_get_frames_num(sprite) == 1) {
            draw_callback = k_component_add_draw_callback(renderer->component, renderer_draw_image, renderer->z_index);
        } else {
            draw_callback = k_component_add_draw_callback(renderer->component, renderer_draw_animation, renderer->z_index);
        }

        if (NULL == draw_callback)
            return -1;

        k_component_del_callback(renderer->draw_callback);
        renderer->draw_callback = draw_callback;

        renderer->sprite    = sprite;
        renderer->timer     = 0.0f;
        renderer->speed     = 1.0f;
        renderer->frame_idx = 0;

        renderer->scaled_w = k_sprite_get_width(renderer->sprite);
        renderer->scaled_h = k_sprite_get_height(renderer->sprite);
        renderer->angle    = 0.0f;

        renderer->transform_flags = transform_none;
    }

    return 0;
}

/* endregion */

/* region [renderer_init] */

static int sprite_renderer_init(struct k_component *component, void *params) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);
    struct k_sprite_renderer_config *config = params;

    renderer->component     = component;
    renderer->draw_callback = NULL;
    renderer->z_index       = config->z_index;

    renderer->sprite    = NULL;
    renderer->timer     = 0.0f;
    renderer->speed     = 1.0f;
    renderer->frame_idx = 0;

    renderer->x = config->x;
    renderer->y = config->y;

    renderer->scaled_w = 0;
    renderer->scaled_h = 0;
    renderer->angle    = 0.0f;

    renderer->transform_flags = transform_none;

    k_sprite_renderer_set_sprite(renderer, config->sprite);
    return 0;
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

    // k_component_type_set_name(spr_rdr, "k/spr-rdr");
    return 0;
}

struct k_sprite_renderer *k_object_add_sprite_renderer(struct k_object *object, const struct k_sprite_renderer_config *config) {

    if (NULL == object || NULL == config)
        return NULL;

    struct k_component *component = k_object_add_component(object, spr_rdr, (void*)config);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

void k_object_del_sprite_renderer(struct k_sprite_renderer *renderer) {
    if (NULL != renderer)
        k_object_del_component(renderer->component);
}

/* endregion */
