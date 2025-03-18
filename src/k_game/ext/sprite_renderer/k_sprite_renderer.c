#include <math.h>
#include "../../core/k_SDL/k_SDL.h" // <- tmp? TODO

#include "k_game.h"

#include "../k_components_def.h"

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

    struct k_component_callback *callback_draw_sprite;
    int z_index;

    struct k_component_callback *callback_draw_debug_rect;
    int debug;

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

/* region [renderer_speed] */

void k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed) {

    if (NULL == renderer->sprite)
        return;

    if (speed <= 0.0f)
        renderer->speed = 0.0f;
    else
        renderer->speed = speed;
}

void k_sprite_renderer_add_speed(struct k_sprite_renderer *renderer, float speed_delta) {
    k_sprite_renderer_set_speed(renderer, renderer->speed + speed_delta);
}

float k_sprite_renderer_get_speed(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0.0f;
    else
        return renderer->speed;
}

/* endregion */

/* region [renderer_transform] */

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

/* region [renderer_draw] */

static void calc_frame_idx(struct k_sprite_renderer *renderer) {
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

static void draw_sprite(struct k_sprite_renderer *renderer) {

    if (transform_none == renderer->transform_flags) {
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

static void draw_animation(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);

    calc_frame_idx(renderer);
    draw_sprite(renderer);
}

static void draw_image(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);

    draw_sprite(renderer);
}

/* endregion */

/* region [renderer_sprite] */

static void renderer_reset(struct k_sprite_renderer *renderer) {

    renderer->timer = 0.0f;
    renderer->speed = 1.0f;
    renderer->frame_idx = 0;

    if (NULL != renderer->sprite) {
        renderer->scaled_w = k_sprite_get_width(renderer->sprite);
        renderer->scaled_h = k_sprite_get_height(renderer->sprite);
    } else {
        renderer->scaled_w = 0;
        renderer->scaled_h = 0;
    }
    renderer->angle = 0.0f;
    renderer->transform_flags = transform_none;
}

int k_sprite_renderer_set_sprite(struct k_sprite_renderer *renderer, struct k_sprite *sprite) {

    if (NULL == renderer->sprite) {
        if (NULL == sprite) {
            return 0;
        }

        struct k_component_callback *draw_callback;
        if (k_sprite_get_frames_num(sprite) == 1)
            draw_callback = k_component_add_draw_callback(renderer->component, draw_image, renderer->z_index);
        else
            draw_callback = k_component_add_draw_callback(renderer->component, draw_animation, renderer->z_index);

        if (NULL == draw_callback)
            return -1;

        renderer->callback_draw_sprite = draw_callback;

        renderer->sprite = sprite;
        renderer_reset(renderer);
    }
    else {
        if (NULL == sprite) {
            k_component_del_callback(renderer->callback_draw_sprite);
            renderer->callback_draw_sprite = NULL;

            renderer->sprite = NULL;
            renderer_reset(renderer);
            return 0;
        }

        struct k_component_callback *draw_callback;
        if (k_sprite_get_frames_num(sprite) == 1) {
            draw_callback = k_component_add_draw_callback(renderer->component, draw_image, renderer->z_index);
        } else {
            draw_callback = k_component_add_draw_callback(renderer->component, draw_animation, renderer->z_index);
        }

        if (NULL == draw_callback)
            return -1;

        k_component_del_callback(renderer->callback_draw_sprite);
        renderer->callback_draw_sprite = draw_callback;

        renderer->sprite = sprite;
        renderer_reset(renderer);
    }

    k_sprite_renderer_set_debug(renderer, renderer->debug);
    return 0;
}

struct k_sprite *k_sprite_renderer_get_sprite(struct k_sprite_renderer *renderer) {
    return renderer->sprite;
}

/* endregion */

/* region [renderer_z_index] */

int k_sprite_renderer_set_z_index(struct k_sprite_renderer *renderer, int z_index) {

    if (NULL == renderer->sprite) {
        renderer->z_index = z_index;
        return 0;
    }

    struct k_component_callback *draw_callback;
    if (k_sprite_get_frames_num(renderer->sprite) == 1) {
        draw_callback = k_component_add_draw_callback(renderer->component, draw_image, z_index);
    } else {
        draw_callback = k_component_add_draw_callback(renderer->component, draw_animation, z_index);
    }

    if (NULL == draw_callback)
        return -1;

    k_component_del_callback(renderer->callback_draw_sprite);
    renderer->callback_draw_sprite = draw_callback;

    renderer->z_index = z_index;
    return 0;
}

int k_sprite_renderer_get_z_index(struct k_sprite_renderer *renderer) {
    return renderer->z_index;
}

/* endregion */

/* region [renderer_debug] */

static void draw_debug_rect(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);

    float scale_x = (float)renderer->scaled_w / (float)k_sprite_get_width(renderer->sprite);
    float scale_y = (float)renderer->scaled_h / (float)k_sprite_get_height(renderer->sprite);

    float w      = (float)renderer->scaled_w;
    float h      = (float)renderer->scaled_h;
    float ox     = scale_x * k_sprite_get_origin_x(renderer->sprite);
    float oy     = scale_y * k_sprite_get_origin_y(renderer->sprite);
    float angle  = renderer->angle;
    int   flip_x = renderer->transform_flags & transform_flip_x;
    int   flip_y = renderer->transform_flags & transform_flip_y;
    float dst_x  = *(renderer->x);
    float dst_y  = *(renderer->y);

    /* ------------------------------------------------------------------------ */

    float x1, y1, x2, y2;
    float x3, y3, x4, y4;
    if (flip_x) {
        x1 = w - ox;
        x2 = -ox;
        x3 = -ox;
        x4 = w - ox;
    } else {
        x1 = -ox;
        x2 = w - ox;
        x3 = w - ox;
        x4 = -ox;
    }
    if (flip_y) {
        y1 = h - oy;
        y2 = h - oy;
        y3 = -oy;
        y4 = -oy;
    } else {
        y1 = -oy;
        y2 = -oy;
        y3 = h - oy;
        y4 = h - oy;
    }

    angle = angle * (3.14159265358979323846f / 180.0f);
    float s = sinf(angle);
    float c = cosf(angle);
    {
        float x_new = x1 * c - y1 * s;
        float y_new = x1 * s + y1 * c;
        x1 = x_new;
        y1 = y_new;

        x_new = x2 * c - y2 * s;
        y_new = x2 * s + y2 * c;
        x2 = x_new;
        y2 = y_new;

        x_new = x3 * c - y3 * s;
        y_new = x3 * s + y3 * c;
        x3 = x_new;
        y3 = y_new;

        x_new = x4 * c - y4 * s;
        y_new = x4 * s + y4 * c;
        x4 = x_new;
        y4 = y_new;
    }

    x1 += dst_x; y1 += dst_y;
    x2 += dst_x; y2 += dst_y;
    x3 += dst_x; y3 += dst_y;
    x4 += dst_x; y4 += dst_y;

    SDL_SetRenderDrawColor(k__window.renderer, 168, 213, 182, 255);
    SDL_RenderDrawLineF(k__window.renderer, x1, y1, x2, y2);
    SDL_RenderDrawLineF(k__window.renderer, x2, y2, x3, y3);
    SDL_RenderDrawLineF(k__window.renderer, x3, y3, x4, y4);
    SDL_RenderDrawLineF(k__window.renderer, x4, y4, x1, y1);

    /* ------------------------------------------------------------------------ */

    float len = 8;
    float len_c = len * c;
    float len_s = len * s;
    x1 = dst_x - len_c;
    y1 = dst_y - len_s;
    x2 = dst_x + len_c;
    y2 = dst_y + len_s;
    x3 = dst_x + len_s;
    y3 = dst_y - len_c;
    x4 = dst_x - len_s;
    y4 = dst_y + len_c;
    SDL_SetRenderDrawColor(k__window.renderer, 168, 213, 182, 255);
    SDL_RenderDrawLineF(k__window.renderer, x1, y1, x2, y2);
    SDL_RenderDrawLineF(k__window.renderer, x3, y3, x4, y4);
}

int k_sprite_renderer_set_debug(struct k_sprite_renderer *renderer, int debug) {

    if (NULL == renderer->sprite) {
        renderer->debug = debug;
        return 0;
    }

#define K_GAME_DEBUG_LAYER 100000

    if (NULL == renderer->callback_draw_debug_rect) {
        if (0 != debug) {
            struct k_component_callback *callback;
            callback = k_component_add_draw_callback(renderer->component, draw_debug_rect, K_SPRITE_RENDERER_DEBUG_DRAW_LAYER);
            if (NULL == callback)
                return -1;

            renderer->callback_draw_debug_rect = callback;
        }
    } else {
        if (0 == debug) {
            k_component_del_callback(renderer->callback_draw_debug_rect);
            renderer->callback_draw_debug_rect = NULL;
        }
    }

    renderer->debug = debug;
    return 0;
}

/* endregion */

/* region [renderer_init] */

static int sprite_renderer_init(struct k_component *component, void *params) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);
    struct k_sprite_renderer_config *config = params;

    renderer->component = component;

    renderer->callback_draw_sprite = NULL;
    renderer->z_index = config->z_index;

    renderer->callback_draw_debug_rect = NULL;
    renderer->debug = 0;

    renderer->x = config->x;
    renderer->y = config->y;

    renderer->sprite = NULL;

    if (NULL == config->sprite) {
        renderer_reset(renderer);
    } else {
        if (0 != k_sprite_renderer_set_sprite(renderer, config->sprite))
            return -1;
    }

    return 0;
}

/* endregion */

/* region [component_define] */

static struct k_component_type *spr_rdr = NULL;

int k__component_def_sprite_renderer(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size = sizeof(struct k_sprite_renderer);
    config.fn_init = sprite_renderer_init;

    spr_rdr = k_component_define(NULL, &config);
    if (NULL == spr_rdr)
        return -1;

    return 0;
}

/* endregion */

/* region [object_add_sprite_renderer] */

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
