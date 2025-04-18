#include <math.h>
#include <assert.h>

#include "k_log.h"

#include "SDL_render.h"

#include "k_game/core/k_canvas.h"

#include "./k_canvas.h"
#include "./k_window.h"

#include "../image/k_image.h"
#include "../sprite/k_sprite.h"

struct k_canvas k__canvas;

static int k__canvas_set_viewport(enum k_canvas_viewport viewport) {

    {
        SDL_Texture *texture = SDL_GetRenderTarget(k__window.renderer);
        if (texture != k__canvas.canvas) {
            SDL_SetRenderTarget(k__window.renderer, k__canvas.canvas);
            k__canvas.current_viewport = K__CANVAS_VIEWPORT_NONE;
        }
    }

    switch (viewport) {
        case K__CANVAS_VIEWPORT_ROOM: {

            if (K__CANVAS_VIEWPORT_ROOM == k__canvas.current_viewport)
                return 0;

            SDL_Rect clip;
            clip.x = (int)k__canvas.room_viewport.x;
            clip.y = (int)k__canvas.room_viewport.y;
            clip.w = (int)k__window.view_w;
            clip.h = (int)k__window.view_h;
            if (SDL_RenderSetViewport(k__window.renderer, &clip)) {
                k_log_error("SDL error: %s", SDL_GetError());
                return -1;
            }

            k__canvas.current_viewport = K__CANVAS_VIEWPORT_ROOM;
            k__canvas.viewport_rect.x = k__canvas.room_viewport.x;
            k__canvas.viewport_rect.y = k__canvas.room_viewport.y;
            k__canvas.viewport_rect.w = k__window.view_w;
            k__canvas.viewport_rect.h = k__window.view_h;

            k__canvas.viewport_w = k__window.view_w;
            k__canvas.viewport_h = k__window.view_h;

            return 0;
        }
        case K__CANVAS_VIEWPORT_UI: {

            if (K__CANVAS_VIEWPORT_UI == k__canvas.current_viewport)
                return 0;

            SDL_Rect clip;
            clip.x = (int)k__canvas.ui_viewport.x;
            clip.y = (int)k__canvas.ui_viewport.y;
            clip.w = (int)k__canvas.ui_viewport.w;
            clip.h = (int)k__canvas.ui_viewport.h;
            if (SDL_RenderSetViewport(k__window.renderer, &clip)) {
                k_log_error("SDL error: %s", SDL_GetError());
                return -1;
            }

            k__canvas.current_viewport = K__CANVAS_VIEWPORT_UI;
            k__canvas.viewport_rect.x = k__canvas.ui_viewport.x;
            k__canvas.viewport_rect.y = k__canvas.ui_viewport.y;
            k__canvas.viewport_rect.w = k__canvas.ui_viewport.w;
            k__canvas.viewport_rect.h = k__canvas.ui_viewport.h;

            k__canvas.viewport_w = k__canvas.ui_viewport.w;
            k__canvas.viewport_h = k__canvas.ui_viewport.h;

            return 0;
        }
        default: {
            assert(0);
            return -1;
        }
    }
}

static void k__canvas_convert_xy(float *x, float *y) {

    switch (k__canvas.current_viewport) {
        case K__CANVAS_VIEWPORT_ROOM: {

            float x_in_room = *x;
            float x_in_view = x_in_room - k__window.view_x;
            *x = x_in_view;

            float y_in_room = *y;
            float y_in_view = y_in_room - k__window.view_y;
            *y = y_in_view;

            return;
        }
        case K__CANVAS_VIEWPORT_UI: {

            float x_in_window = *x;
            float x_in_ui = x_in_window;
            *x = x_in_ui;

            float y_in_window = *y;
            float y_in_ui = y_in_window;
            *y = y_in_ui;

            return;
        }
        default: {
            assert(0);
        }
    }
}

/* region [set_color] */

void k_canvas_set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(k__window.renderer, r, g, b, a);
}

void k_canvas_get_draw_color(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a) {
    SDL_GetRenderDrawColor(k__window.renderer, r, g, b, a);
}

/* endregion */

/* region [clear] */

int k_canvas_room_clear(void) {

    if (0 != k__canvas_set_viewport(K__CANVAS_VIEWPORT_ROOM))
        return -1;

    SDL_SetRenderDrawBlendMode(k__window.renderer, SDL_BLENDMODE_NONE);

    if (0 != SDL_RenderFillRectF(k__window.renderer, NULL)) {
        k_log_error("SDL error: %s", SDL_GetError());
        return -1;
    }

    SDL_SetRenderDrawBlendMode(k__window.renderer, SDL_BLENDMODE_BLEND);

    return 0;
}

int k_canvas_ui_clear(void) {

    if (0 != k__canvas_set_viewport(K__CANVAS_VIEWPORT_UI))
        return -1;

    SDL_SetRenderDrawBlendMode(k__window.renderer, SDL_BLENDMODE_NONE);

    if (0 != SDL_RenderFillRectF(k__window.renderer, NULL)) {
        k_log_error("SDL error: %s", SDL_GetError());
        return -1;
    }

    SDL_SetRenderDrawBlendMode(k__window.renderer, SDL_BLENDMODE_BLEND);

    return 0;
}

/* endregion */

/* region [cull] */

int k__canvas_cull_point(float x, float y) {
    return x < 0.0f
        || y < 0.0f
        || x > k__canvas.viewport_w
        || y > k__canvas.viewport_h;
}

int k__canvas_cull_line(float x1, float y1, float x2, float y2) {
    return ((x1 < x2) ? (x2 < 0.0f || 0.0f + k__canvas.viewport_w < x1) : (x1 < 0.0f || 0.0f + k__canvas.viewport_w < x2))
        || ((y1 < y2) ? (y2 < 0.0f || 0.0f + k__canvas.viewport_h < y1) : (y1 < 0.0f || 0.0f + k__canvas.viewport_h < y2));
}

int k__canvas_cull_rect(float x, float y, float w, float h) {
    return x > k__canvas.viewport_w
        || y > k__canvas.viewport_h
        || x + w < 0.0f
        || y + h < 0.0f;
}

int k__canvas_cull_circle(float cx, float cy, float r) {
    return cx + r < 0.0f
        || cy + r < 0.0f
        || k__canvas.viewport_w < cx - r
        || k__canvas.viewport_h < cy - r;
}

/* endregion */

/* region [draw_graphics] */

#define k__canvas_buf_capacity(arr) (sizeof(arr) / sizeof(arr[0]))

/* region [draw_point] */

static int k__canvas_draw_point(enum k_canvas_viewport viewport, float x, float y) {

    if (0 != k__canvas_set_viewport(viewport))
        return -1;

    k__canvas_convert_xy(&x, &y);

    if (k__canvas_cull_point(x, y))
        return 0;

    if (0 != SDL_RenderDrawPointF(k__window.renderer, x, y)) {
        k_log_error("SDL error: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int k_canvas_room_draw_point(float x, float y) {
    return k__canvas_draw_point(K__CANVAS_VIEWPORT_ROOM, x, y);
}

int k_canvas_ui_draw_point(float x, float y) {
    return k__canvas_draw_point(K__CANVAS_VIEWPORT_UI, x, y);
}

/* endregion */

/* region [draw_points] */

static int k__canvas_draw_points(enum k_canvas_viewport viewport, const struct k_float_point *points, size_t points_num) {

    if (NULL == points)
        return -1;

    if (points_num == 0)
        return 0;

    if (0 != k__canvas_set_viewport(viewport))
        return -1;

    SDL_FPoint buf[80];
    int buf_size = 0;

    size_t count = 0;
    while (count < points_num) {

        float x = points[count].x;
        float y = points[count].y;
        count++;

        k__canvas_convert_xy(&x, &y);

        if (k__canvas_cull_point(x, y))
            continue;

        buf[buf_size].x = x;
        buf[buf_size].y = y;
        buf_size++;

        if (buf_size >= k__canvas_buf_capacity(buf)) {

            if (0 != SDL_RenderDrawPointsF(k__window.renderer, buf, buf_size)) {
                k_log_error("SDL error: %s", SDL_GetError());
                return -1;
            }

            buf_size = 0;
        }
    }

    if (0 != buf_size) {
        if (0 != SDL_RenderDrawPointsF(k__window.renderer, buf, buf_size)) {
            k_log_error("SDL error: %s", SDL_GetError());
            return -1;
        }
    }

    return 0;
}

int k_room_canvas_draw_points(const struct k_float_point *points, size_t points_num) {
    return k__canvas_draw_points(K__CANVAS_VIEWPORT_ROOM, points, points_num);
}

int k_ui_canvas_draw_points(const struct k_float_point *points, size_t points_num) {
    return k__canvas_draw_points(K__CANVAS_VIEWPORT_UI, points, points_num);
}

/* endregion */

/* region [draw_line] */

static int k__canvas_draw_line(enum k_canvas_viewport viewport, float x1, float y1, float x2, float y2) {

    if (0 != k__canvas_set_viewport(viewport))
        return -1;

    k__canvas_convert_xy(&x1, &y1);
    k__canvas_convert_xy(&x2, &y2);

    if (k__canvas_cull_line(x1, y1, x2, y2))
        return 0;

    if (0 != SDL_RenderDrawLineF(k__window.renderer, x1, y1, x2, y2)) {
        k_log_error("SDL error: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int k_canvas_room_draw_line(float x1, float y1, float x2, float y2) {
    return k__canvas_draw_line(K__CANVAS_VIEWPORT_ROOM, x1, y1, x2, y2);
}

int k_canvas_ui_draw_line(float x1, float y1, float x2, float y2) {
    return k__canvas_draw_line(K__CANVAS_VIEWPORT_UI, x1, y1, x2, y2);
}

/* endregion */

/* region [draw_lines] */

static int k__canvas_draw_lines(enum k_canvas_viewport viewport, const struct k_float_point *points, size_t points_num) {

    if (NULL == points)
        return -1;

    if (points_num <= 1)
        return 0;

    if (0 != k__canvas_set_viewport(viewport))
        return -1;

    SDL_FPoint buf[80];
    int buf_size = 0;

    size_t count = 0;
    while (1) {

        float x = points[count].x;
        float y = points[count].y;
        count++;

        k__canvas_convert_xy(&x, &y);

        buf[buf_size].x = x;
        buf[buf_size].y = y;
        buf_size++;

        if (count == points_num) {

            if (0 != SDL_RenderDrawLinesF(k__window.renderer, buf, buf_size)) {
                k_log_error("SDL error: %s", SDL_GetError());
                return -1;
            }

            break;
        }
        else if (buf_size >= k__canvas_buf_capacity(buf)) {

            if (0 != SDL_RenderDrawLinesF(k__window.renderer, buf, buf_size)) {
                k_log_error("SDL error: %s", SDL_GetError());
                return -1;
            }

            if (count == points_num)
                break;

            buf[0].x = buf[k__canvas_buf_capacity(buf) - 1].x;
            buf[0].y = buf[k__canvas_buf_capacity(buf) - 1].y;
            buf_size = 1;
        }
    }

    return 0;
}

int k_canvas_room_draw_lines(const struct k_float_point *points, size_t points_num) {
    return k__canvas_draw_lines(K__CANVAS_VIEWPORT_ROOM, points, points_num);
}

int k_canvas_ui_draw_lines(const struct k_float_point *points, size_t points_num) {
    return k__canvas_draw_lines(K__CANVAS_VIEWPORT_UI, points, points_num);
}

/* endregion */

/* region [draw_rect] */

static int k__canvas_draw_rect(enum k_canvas_viewport viewport, float x, float y, float w, float h) {

    if (w <= 0.0f || h <= 0.0f)
        return 0;

    if (0 != k__canvas_set_viewport(viewport))
        return -1;

    k__canvas_convert_xy(&x, &y);

    if (x + w < k__canvas.viewport_rect.x || k__canvas.viewport_rect.x + k__canvas.viewport_rect.w < x)
        return 0;
    if (y + h < k__canvas.viewport_rect.x || k__canvas.viewport_rect.y + k__canvas.viewport_rect.h < y)
        return 0;

    SDL_FRect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    if (0 != SDL_RenderDrawRectF(k__window.renderer, &rect)) {
        k_log_error("SDL error: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int k_canvas_room_draw_rect(float x, float y, float w, float h) {
    return k__canvas_draw_rect(K__CANVAS_VIEWPORT_ROOM, x, y, w, h);
}

int k_canvas_ui_draw_rect(float x, float y, float w, float h) {
    return k__canvas_draw_rect(K__CANVAS_VIEWPORT_UI, x, y, w, h);
}

/* endregion */

/* region [fill_rect] */

static int k__canvas_fill_rect(enum k_canvas_viewport viewport, float x, float y, float w, float h) {

    if (w <= 0.0f || h <= 0.0f)
        return 0;

    if (0 != k__canvas_set_viewport(viewport))
        return -1;

    k__canvas_convert_xy(&x, &y);

    if (k__canvas_cull_rect(x, y, w, h))
        return 0;

    SDL_FRect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    if (0 != SDL_RenderFillRectF(k__window.renderer, &rect)) {
        k_log_error("SDL error: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int k_canvas_room_fill_rect(float x, float y, float w, float h) {
    return k__canvas_fill_rect(K__CANVAS_VIEWPORT_ROOM, x, y, w, h);
}

int k_canvas_ui_fill_rect(float x, float y, float w, float h) {
    return k__canvas_fill_rect(K__CANVAS_VIEWPORT_UI, x, y, w, h);
}

/* endregion */

/* region [draw_circle] */

static int k__canvas_draw_circle(enum k_canvas_viewport viewport, float cx, float cy, float r) {

    if (r <= 0.0f)
        return 0;

    if (0 != k__canvas_set_viewport(viewport))
        return -1;

    k__canvas_convert_xy(&cx, &cy);

    if (k__canvas_cull_circle(cx, cy, r))
        return 0;

    /* 数组大小应是 8 的倍数，因为每轮循环都会往 buf 中添加 8 个点 */
    SDL_FPoint buf[80];
    int buf_size = 0;

    float err = 0.0f;
    float x = r;
    float y = 0.0f;
    while (1) {

        buf[buf_size + 0] = (SDL_FPoint){ cx + x, cy + y };
        buf[buf_size + 1] = (SDL_FPoint){ cx + y, cy + x };
        buf[buf_size + 2] = (SDL_FPoint){ cx - y, cy + x };
        buf[buf_size + 3] = (SDL_FPoint){ cx - x, cy + y };
        buf[buf_size + 4] = (SDL_FPoint){ cx - x, cy - y };
        buf[buf_size + 5] = (SDL_FPoint){ cx - y, cy - x };
        buf[buf_size + 6] = (SDL_FPoint){ cx + y, cy - x };
        buf[buf_size + 7] = (SDL_FPoint){ cx + x, cy - y };
        buf_size += 8;

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        } else {
            x -= 1;
            err -= 2 * x + 1;
        }

        if (x < y) {
            if (0 != SDL_RenderDrawPointsF(k__window.renderer, buf, buf_size)) {
                k_log_error("SDL error: %s", SDL_GetError());
                return -1;
            }

            break;
        }
        else if (buf_size >= k__canvas_buf_capacity(buf)) {
            if (0 != SDL_RenderDrawPointsF(k__window.renderer, buf, buf_size)) {
                k_log_error("SDL error: %s", SDL_GetError());
                return -1;
            }

            buf_size = 0;
        }
    }

    return 0;
}

int k_canvas_room_draw_circle(float cx, float cy, float r) {
    return k__canvas_draw_circle(K__CANVAS_VIEWPORT_ROOM, cx, cy, r);
}

int k_canvas_ui_draw_circle(float cx, float cy, float r) {
    return k__canvas_draw_circle(K__CANVAS_VIEWPORT_UI, cx, cy, r);
}

/* endregion */

/* endregion */

/* region [draw_image] */

static int k__canvas_draw_image(enum k_canvas_viewport viewport, struct k_image *image, const struct k_int_rect *src_rect, float x, float y, struct k_canvas_draw_image_options *options) {

    if (NULL == image)
        return -1;

    SDL_Rect src;
    if (NULL == src_rect) {
        src.x = 0;
        src.y = 0;
        src.w = image->image_w;
        src.h = image->image_h;
    }
    else {
        if (src_rect->w <= 0 || src_rect->h <= 0)
            return -1;

        src.x = src_rect->x;
        src.y = src_rect->y;
        src.w = src_rect->w;
        src.h = src_rect->h;
    }

    if (0 != k__canvas_set_viewport(viewport))
        return -1;

    k__canvas_convert_xy(&x, &y);

    if (NULL == options) {

        SDL_FRect dst;
        dst.x = x;
        dst.y = y;
        dst.w = (float)image->image_w;
        dst.h = (float)image->image_h;

        if (k__canvas_cull_rect(dst.x, dst.y, dst.w, dst.h))
            return 0;

        if (0 != SDL_RenderCopyF(k__window.renderer, image->texture, &src, &dst)) {
            k_log_error("SDL error: %s", SDL_GetError());
            return -1;
        }
    }
    else {
        if (options->scaled_w <= 0.0f || options->scaled_h <= 0.0f)
            return 0;

        if (k__canvas_cull_circle(x, y, (options->scaled_w + options->scaled_h) / 2.0f))
            return 0;

        SDL_FRect dst;
        dst.x = x;
        dst.y = y;
        dst.w = options->scaled_w;
        dst.h = options->scaled_h;

        SDL_FPoint center;
        center.x = options->pivot_x;
        center.y = options->pivot_y;

        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (options->flip_x) { flip |= SDL_FLIP_HORIZONTAL; }
        if (options->flip_y) { flip |= SDL_FLIP_VERTICAL;   }

        if (0 != SDL_RenderCopyExF(k__window.renderer, image->texture, &src, &dst, options->angle, &center, flip)) {
            k_log_error("SDL error: %s", SDL_GetError());
            return -1;
        }
    }

    return 0;
}

int k_canvas_room_draw_image(struct k_image *image, const struct k_int_rect *src_rect, float x, float y, struct k_canvas_draw_image_options *options) {
    return k__canvas_draw_image(K__CANVAS_VIEWPORT_ROOM, image, src_rect, x, y, options);
}

int k_canvas_ui_draw_image(struct k_image *image, const struct k_int_rect *src_rect, float x, float y, struct k_canvas_draw_image_options *options) {
    return k__canvas_draw_image(K__CANVAS_VIEWPORT_UI, image, src_rect, x, y, options);
}

/* endregion */

/* region [draw_sprite] */

static int k__canvas_draw_sprite(enum k_canvas_viewport viewport, struct k_sprite *sprite, size_t frame_idx, float x, float y, struct k_canvas_draw_sprite_options *options) {

    if (NULL == sprite)
        return -1;

    if (sprite->frames_num <= frame_idx)
        return -1;

    if (0 != k__canvas_set_viewport(viewport))
        return -1;

    k__canvas_convert_xy(&x, &y);

    struct k_sprite_frame *frame = &sprite->frames[frame_idx];

    if (NULL == options) {

        SDL_FRect dst;
        dst.x = x - sprite->origin_x;
        dst.y = y - sprite->origin_y;
        dst.w = (float)sprite->sprite_w;
        dst.h = (float)sprite->sprite_h;

        if (k__canvas_cull_rect(dst.x, dst.y, dst.w, dst.h))
            return 0;

        SDL_Rect src;
        src.x = frame->offset_x;
        src.y = frame->offset_y;
        src.w = sprite->sprite_w;
        src.h = sprite->sprite_h;

        if (0 != SDL_RenderCopyF(k__window.renderer, frame->image->texture, &src, &dst)) {
            k_log_error("SDL error: %s", SDL_GetError());
            return -1;
        }
    }
    else {
        if (options->scaled_w <= 0.0f || options->scaled_h <= 0.0f)
            return 0;

        float scala_x  = options->scaled_w / (float)sprite->sprite_w;
        float scala_y  = options->scaled_h / (float)sprite->sprite_h;
        float origin_x = scala_x * sprite->origin_x;
        float origin_y = scala_y * sprite->origin_y;

        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (options->flip_x) {
            origin_x = options->scaled_w - origin_x;
            flip |= SDL_FLIP_HORIZONTAL;
        }
        if (options->flip_y) {
            origin_y = options->scaled_h - origin_y;
            flip |= SDL_FLIP_VERTICAL;
        }

        {
            float half_w = options->scaled_w / 2.0f;
            float half_h = options->scaled_h / 2.0f;
            float R = half_w + half_h + fabsf(origin_x - half_w) + fabsf(origin_y - half_h);

            if (k__canvas_cull_circle(x, y, R))
                return 0;
        }

        SDL_Rect src;
        src.x = frame->offset_x;
        src.y = frame->offset_y;
        src.w = sprite->sprite_w;
        src.h = sprite->sprite_h;

        SDL_FRect dst;
        dst.x = x - origin_x;
        dst.y = y - origin_y;
        dst.w = options->scaled_w;
        dst.h = options->scaled_h;

        SDL_FPoint center;
        center.x = origin_x;
        center.y = origin_y;

        if (0 != SDL_RenderCopyExF(k__window.renderer, frame->image->texture, &src, &dst, options->angle, &center, flip)) {
            k_log_error("SDL error: %s", SDL_GetError());
            return -1;
        }
    }

    return 0;
}

int k_canvas_room_draw_sprite(struct k_sprite *sprite, size_t frame_idx, float x, float y, struct k_canvas_draw_sprite_options *options) {
    return k__canvas_draw_sprite(K__CANVAS_VIEWPORT_ROOM, sprite, frame_idx, x, y, options);
}

int k_canvas_ui_draw_sprite(struct k_sprite *sprite, size_t frame_idx, float x, float y, struct k_canvas_draw_sprite_options *options) {
    return k__canvas_draw_sprite(K__CANVAS_VIEWPORT_UI, sprite, frame_idx, x, y, options);
}

/* endregion */

/* region [present] */

void k__canvas_present(void) {

    SDL_SetRenderTarget(k__window.renderer, NULL);

    SDL_Rect room_view;
    room_view.x = (int)(k__canvas.room_viewport.x);
    room_view.y = (int)(k__canvas.room_viewport.y);
    room_view.w = (int)(k__window.view_w);
    room_view.h = (int)(k__window.view_h);
    SDL_RenderCopyF(k__window.renderer, k__canvas.canvas, &room_view, NULL);

    SDL_Rect ui;
    ui.x = (int)(k__canvas.ui_viewport.x);
    ui.y = (int)(k__canvas.ui_viewport.y);
    ui.w = (int)(k__canvas.ui_viewport.w);
    ui.h = (int)(k__canvas.ui_viewport.h);
    SDL_RenderCopyF(k__window.renderer, k__canvas.canvas, &ui, NULL);

    SDL_RenderPresent(k__window.renderer);

    SDL_SetRenderTarget(k__window.renderer, k__canvas.canvas);
    k__canvas.current_viewport = K__CANVAS_VIEWPORT_NONE;
}

/* endregion */
