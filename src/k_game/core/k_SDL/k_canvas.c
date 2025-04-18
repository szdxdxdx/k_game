#include <math.h>

#include "k_log.h"

#include "SDL_render.h"

#include "k_game/core/k_canvas.h"

#include "./k_canvas.h"
#include "./k_window.h"

#include "../image/k_image.h"
#include "../sprite/k_sprite.h"

struct k_canvas k__canvas;

/* region [set_color] */

void k_canvas_set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(k__window.renderer, r, g, b, a);
}

void k_canvas_get_draw_color(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a) {
    SDL_GetRenderDrawColor(k__window.renderer, r, g, b, a);
}

/* endregion */

/* region [draw_graphics] */

int k_canvas_room_clear(void) {

    if (0 != SDL_RenderClear(k__window.renderer)) {
        k_log_error("SDL error: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

#define k__canvas_buf_capacity(arr) (sizeof(arr) / sizeof(arr[0]))

int k_canvas_room_draw_point(float x, float y) {

    x -= k__window.view_x;
    y -= k__window.view_y;

    if (x < 0.0f || k__window.view_w < x)
        return 0;
    if (y < 0.0f || k__window.view_h < y)
        return 0;

    if (0 != SDL_RenderDrawPointF(k__window.renderer, x, y)) {
        k_log_error("SDL error: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int k_canvas_room_draw_points(const struct k_float_point *points, size_t points_num) {

    if (NULL == points)
        return -1;

    if (points_num == 0)
        return 0;

    SDL_FPoint buf[80];
    int buf_size = 0;

    size_t count = 0;
    while (count < points_num) {

        float x = points[count].x - k__window.view_x;
        float y = points[count].y - k__window.view_y;
        count++;

        if (x < 0.0f || k__window.view_w < x)
            continue;
        if (y < 0.0f || k__window.view_h < y)
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

int k_canvas_room_draw_line(float x1, float y1, float x2, float y2) {

    x1 -= k__window.view_x;
    y1 -= k__window.view_y;
    x2 -= k__window.view_x;
    y2 -= k__window.view_y;

    if ((x1 < x2) ? (x2 < 0.0f || k__window.view_w < x1) : (x1 < 0.0f || k__window.view_w < x2))
        return 0;
    if ((y1 < y2) ? (y2 < 0.0f || k__window.view_h < y1) : (y1 < 0.0f || k__window.view_h < y2))
        return 0;

    if (0 != SDL_RenderDrawLineF(k__window.renderer, x1, y1, x2, y2)) {
        k_log_error("SDL error: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int k_canvas_room_draw_lines(const struct k_float_point *points, size_t points_num) {

    if (NULL == points)
        return -1;

    if (points_num <= 1)
        return 0;

    SDL_FPoint buf[80];
    int buf_size = 0;

    size_t count = 0;
    while (1) {

        buf[buf_size].x = points[count].x - k__window.view_x;
        buf[buf_size].y = points[count].y - k__window.view_y;
        buf_size++;
        count++;

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

int k_canvas_room_draw_rect(float x, float y, float w, float h) {

    if (w <= 0.0f || h <= 0.0f)
        return 0;

    x -= k__window.view_x;
    y -= k__window.view_y;

    if (k__window.view_w < x || x + w < 0.0f)
        return 0;
    if (k__window.view_h < y || y + h < 0.0f)
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

int k_canvas_room_fill_rect(float x, float y, float w, float h) {

    if (w <= 0.0f || h <= 0.0f)
        return 0;

    x -= k__window.view_x;
    y -= k__window.view_y;

    if (k__window.view_w < x || x + w < 0.0f)
        return 0;
    if (k__window.view_h < y || y + h < 0.0f)
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

int k_canvas_room_draw_circle(float cx, float cy, float r) {

    if (r <= 0.0f)
        return 0;

    cx -= k__window.view_x;
    cy -= k__window.view_y;

    if (k__window.view_w < cx - r || cx + r < 0.0f) return 0;
    if (k__window.view_h < cy - r || cy + r < 0.0f) return 0;

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

/* endregion */

/* region [draw_image] */

int k_canvas_room_draw_image(struct k_image *image, const struct k_int_rect *src_rect, float x, float y, struct k_canvas_draw_image_options *options) {

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

    x -= k__window.view_x;
    y -= k__window.view_y;

    if (NULL == options) {

        SDL_FRect dst;
        dst.x = x;
        dst.y = y;
        dst.w = (float)image->image_w;
        dst.h = (float)image->image_h;

        if (k__window.view_w < dst.x || dst.x + dst.w < 0.0f)
            return 0;
        if (k__window.view_h < dst.y || dst.y + dst.h < 0.0f)
            return 0;

        if (0 != SDL_RenderCopyF(k__window.renderer, image->texture, &src, &dst)) {
            k_log_error("SDL error: %s", SDL_GetError());
            return -1;
        }
    }
    else {
        if (options->scaled_w <= 0.0f || options->scaled_h <= 0.0f)
            return 0;

        float R = (options->scaled_w + options->scaled_h) / 2.0f;
        if (x + R < 0.0f || k__window.view_w < x - R)
            return 0;
        if (y + R < 0.0f || k__window.view_h < y - R)
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

/* endregion */

/* region [draw_sprite] */

int k_canvas_room_draw_sprite(struct k_sprite *sprite, size_t frame_idx, float x, float y, struct k_canvas_draw_sprite_options *options) {

    if (NULL == sprite)
        return -1;

    if (sprite->frames_num <= frame_idx)
        return -1;

    x -= k__window.view_x;
    y -= k__window.view_y;

    struct k_sprite_frame *frame = &sprite->frames[frame_idx];

    if (NULL == options) {

        SDL_FRect dst;
        dst.x = x - sprite->origin_x;
        dst.y = y - sprite->origin_y;
        dst.w = (float)sprite->sprite_w;
        dst.h = (float)sprite->sprite_h;

        if (k__window.view_w < dst.x || dst.x + dst.w < 0.0f)
            return 0;
        if (k__window.view_h < dst.y || dst.y + dst.h < 0.0f)
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
            if (x + R < 0.0f || k__window.view_w < x - R)
                return 0;
            if (y + R < 0.0f || k__window.view_h < y - R)
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

/* endregion */
