#include <assert.h>

#include "k_log.h"

#include "SDL_render.h"

#include "k_game/core/k_canvas.h"
#include "./k_window.h"

#include "../image/k_image.h"

void k_canvas_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(k__window.renderer, a, g, b, a);
}

int k_canvas_draw_point(float x, float y) {
    x -= k__window.view_x;
    y -= k__window.view_y;
    SDL_RenderDrawPointF(k__window.renderer, x, y);

    return 0;
}

int k_canvas_draw_line(float x1, float y1, float x2, float y2) {
    x1 -= k__window.view_x;
    y1 -= k__window.view_y;
    x2 -= k__window.view_x;
    y2 -= k__window.view_y;
    SDL_RenderDrawLineF(k__window.renderer, x1, y1, x2, y2);

    return 0;
}

#define k__array_len(arr) (sizeof(arr) / sizeof(arr[0]))

int k_canvas_draw_lines(const struct k_float_point *points, size_t points_num) {
    assert(NULL != points);

    if (points_num <= 1)
        return -1;

    SDL_FPoint buf[80];
    int buf_size = 0;

    size_t count = 0;
    while (1) {

        buf[buf_size].x = points[count].x - k__window.view_x;
        buf[buf_size].y = points[count].y - k__window.view_y;
        buf_size++;
        count++;

        if (count == points_num) {
            SDL_RenderDrawLinesF(k__window.renderer, buf, buf_size);
            break;
        }
        else if (buf_size == k__array_len(buf)) {
            SDL_RenderDrawLinesF(k__window.renderer, buf, buf_size);

            if (count == points_num) {
                break;
            }
            else {
                buf[0].x = buf[k__array_len(buf) - 1].x;
                buf[0].y = buf[k__array_len(buf) - 1].y;
                buf_size = 1;
            }
        }
    }

    return 0;
}

int k_canvas_draw_rect(float x, float y, float w, float h) {
    SDL_FRect rect;
    rect.x = x - k__window.view_x;
    rect.y = y - k__window.view_y;
    rect.w = w;
    rect.h = h;
    SDL_RenderDrawRectF(k__window.renderer, &rect);

    return 0;
}

int k_canvas_draw_circle(float cx, float cy, float r) {
    cx -= k__window.view_x;
    cy -= k__window.view_y;

    SDL_FPoint buf[80]; /* 数组大小应是 8 的倍数 */
    int buf_size = 0;

    float x   = r;
    float y   = 0.0f;
    float err = 0.0f;
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
            SDL_RenderDrawPointsF(k__window.renderer, buf, buf_size);
            break;
        }
        else if (buf_size == k__array_len(buf)) {
            SDL_RenderDrawPointsF(k__window.renderer, buf, buf_size);
            buf_size = 0;
        }
    }

    return 0;
}

int k_canvas_draw_image(struct k_image *image, const struct k_int_rect *src_rect, float x, float y, struct k_canvas_draw_image_options *options) {

    if (NULL == image)
        return -1;

    /* TODO 检查输出的矩形有没有在视野范围内
     */

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

    if (NULL == options) {

        SDL_FRect dst;
        dst.x = x - k__window.view_x;
        dst.y = y - k__window.view_y;
        dst.w = (float)image->image_w;
        dst.h = (float)image->image_h;

        if (0 != SDL_RenderCopyF(k__window.renderer, image->texture, &src, &dst)) {
            k_log_error("Failed to draw image, SDL error: %s", SDL_GetError());
            return -1;
        }
    }
    else {

        SDL_FRect dst;
        dst.x = x - k__window.view_x;
        dst.y = y - k__window.view_y;
        dst.w = options->scaled_w;
        dst.h = options->scaled_h;

        if (dst.w <= 0.0f || dst.h <= 0.0f)
            return 0;

        SDL_FPoint center;
        center.x = options->pivot_x;
        center.y = options->pivot_y;

        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (options->flip_x) { flip |= SDL_FLIP_HORIZONTAL; }
        if (options->flip_y) { flip |= SDL_FLIP_VERTICAL;   }

        if (0 != SDL_RenderCopyExF(k__window.renderer, image->texture, &src, &dst, options->angle, &center, flip)) {
            k_log_error("Failed to draw image, SDL error: %s", SDL_GetError());
            return -1;
        }
    }

    return 0;
}
