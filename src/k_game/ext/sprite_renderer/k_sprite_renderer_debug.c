#include <math.h>

#include "k_game/core/k_canvas.h"

#include "./_internal.h"

static void k__sprite_renderer_debug_draw(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);

    k_canvas_set_color(255, 102, 0, 255);

    float dst_x = *(renderer->x);
    float dst_y = *(renderer->y);

    float angle_sin;
    float angle_cos;
    if (NULL != renderer->sprite) {
        float angle = renderer->angle * (3.1415926f / 180.0f);
        angle_sin = sinf(angle);
        angle_cos = cosf(angle);
        goto draw_rect;
    }
    else {
        angle_sin = 1.0f;
        angle_cos = 0.0f;
        goto draw_position;
    }

draw_rect:
    {
        float w       = renderer->scaled_w;
        float h       = renderer->scaled_h;
        float scale_x = renderer->scaled_w / (float)k_sprite_get_width(renderer->sprite);
        float scale_y = renderer->scaled_h / (float)k_sprite_get_height(renderer->sprite);
        float ox      = scale_x * k_sprite_get_origin_x(renderer->sprite);
        float oy      = scale_y * k_sprite_get_origin_y(renderer->sprite);
        int   flip_x  = renderer->transform_flags & transform_flip_x;
        int   flip_y  = renderer->transform_flags & transform_flip_y;

        float x1, x2, x3, x4;
        float y1, y2, y3, y4;

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

        float x_new;
        float y_new;

        x_new = x1 * angle_cos - y1 * angle_sin;
        y_new = x1 * angle_sin + y1 * angle_cos;
        x1 = x_new + dst_x;
        y1 = y_new + dst_y;

        x_new = x2 * angle_cos - y2 * angle_sin;
        y_new = x2 * angle_sin + y2 * angle_cos;
        x2 = x_new + dst_x;
        y2 = y_new + dst_y;

        x_new = x3 * angle_cos - y3 * angle_sin;
        y_new = x3 * angle_sin + y3 * angle_cos;
        x3 = x_new + dst_x;
        y3 = y_new + dst_y;

        x_new = x4 * angle_cos - y4 * angle_sin;
        y_new = x4 * angle_sin + y4 * angle_cos;
        x4 = x_new + dst_x;
        y4 = y_new + dst_y;

        struct k_float_point points[5] = {
            { x1, y1 },
            { x2, y2 },
            { x3, y3 },
            { x4, y4 },
            { x1, y1 },
        };
        k_canvas_draw_lines(points, 5);
    }

draw_position:
    {
        float x1, x2, x3, x4;
        float y1, y2, y3, y4;

        float len = 6;
        float len_c = len * angle_cos;
        float len_s = len * angle_sin;
        x1 = dst_x - len_c;
        y1 = dst_y - len_s;
        x2 = dst_x + len_c;
        y2 = dst_y + len_s;
        x3 = dst_x + len_s;
        y3 = dst_y - len_c;
        x4 = dst_x - len_s;
        y4 = dst_y + len_c;
        k_canvas_draw_line(x1, y1, x2, y2);
        k_canvas_draw_line(x3, y3, x4, y4);
    }
}

int k_sprite_renderer_set_debug(struct k_sprite_renderer *renderer, int debug) {

    if (0 == debug) {

        if (NULL == renderer->cb_debug_draw)
            return 0;

        k_component_del_callback(renderer->cb_debug_draw);
        renderer->cb_debug_draw = NULL;
        return 0;
    }
    else {

        if (NULL != renderer->cb_debug_draw)
            return 0;

        renderer->cb_debug_draw = k_component_add_draw_callback(
            renderer->component,
            k__sprite_renderer_debug_draw,
            K__SPRITE_RENDERER_DEBUG_Z_GROUP,
            K__SPRITE_RENDERER_DEBUG_Z_LAYER
        );

        if (NULL == renderer->cb_debug_draw)
            return -1;

        return 0;
    }
}
