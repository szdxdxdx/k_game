#ifndef K_CANVAS_H
#define K_CANVAS_H

#include <stdint.h>

void k_canvas_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void k_canvas_draw_point(float x, float y);

void k_canvas_draw_line(float x1, float y1, float x2, float y2);

struct k_float_point {
    float x;
    float y;
};

void k_canvas_draw_lines(const struct k_float_point *points, size_t points_num);

void k_canvas_draw_rect(float x, float y, float w, float h);

void k_canvas_draw_circle(float cx, float cy, float r);

#endif
