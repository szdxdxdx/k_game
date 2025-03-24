#include <math.h>

#include "../../core/k_SDL/_public.h"

#include "./_internal.h"

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
        x1 = ox;
        x2 = ox - w;
        x3 = ox - w;
        x4 = ox;
    } else {
        x1 = w - ox;
        x2 = -ox;
        x3 = -ox;
        x4 = w - ox;
    }
    if (flip_y) {
        y1 = oy;
        y2 = oy;
        y3 = oy - h;
        y4 = oy - h;
    } else {
        y1 = h - oy;
        y2 = h - oy;
        y3 = -oy;
        y4 = -oy;
    }

    angle *= (3.14159265358979323846f / 180.0f);
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

    SDL_SetRenderDrawColor(k__window.renderer, 76, 201, 176, 255);
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
    SDL_SetRenderDrawColor(k__window.renderer, 76, 201, 176, 255);
    SDL_RenderDrawLineF(k__window.renderer, x1, y1, x2, y2);
    SDL_RenderDrawLineF(k__window.renderer, x3, y3, x4, y4);
}

int k_sprite_renderer_set_debug(struct k_sprite_renderer *renderer, int debug) {

    if (NULL == renderer->sprite) {

        if (NULL != renderer->callback_draw_rect) {
            k_component_del_callback(renderer->callback_draw_rect);
            renderer->callback_draw_rect = NULL;
        }

        renderer->debug = debug;
        return 0;
    }

    if (NULL == renderer->callback_draw_rect) {
        if (0 != debug) {
            struct k_component_callback *callback;
            callback = k_component_add_draw_callback(renderer->component, draw_debug_rect, K_SPRITE_RENDERER_DEBUG_Z_GROUP, K_SPRITE_RENDERER_DEBUG_Z_LAYER);
            if (NULL == callback)
                return -1;

            renderer->callback_draw_rect = callback;
        }
    } else {
        if (0 == debug) {
            k_component_del_callback(renderer->callback_draw_rect);
            renderer->callback_draw_rect = NULL;
        }
    }

    renderer->debug = debug;
    return 0;
}
