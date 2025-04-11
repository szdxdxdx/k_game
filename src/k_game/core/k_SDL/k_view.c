#include <math.h>

#include "k_log.h"

#include "k_game/core/k_view.h"
#include "./k_window.h"
#include "./k_mouse.h"

#include "../room/k_room.h"

int k_view_fit_rect(float w, float h) {

    if (w <= 0 || isnan(w) || isinf(w) || h <= 0 || isnan(h) || isinf(h)){
        k_log_error("invalid parameters `w`=%f, `h`=%f", w, h);
        return -1;
    }

    float aspect = k__window.window_aspect_ratio;

    float view_new_w;
    float view_new_h;

    float w_ = h * aspect;
    if (w <= w_) {
        view_new_w = w_;
        view_new_h = h;
    } else {
        view_new_w = w;
        view_new_h = w / aspect;
    }

    float view_max_w = (float)k__window.canvas_w;
    if (view_max_w > k__current_room->room_w) {
        view_max_w = k__current_room->room_w;
    }

    float view_max_h = (float)k__window.canvas_h;
    if (view_max_h > k__current_room->room_h) {
        view_max_h = k__current_room->room_h;
    }

    w_ = view_max_h * aspect;
    if (w_ < view_max_w) {
        view_max_w = w_;
    } else {
        view_max_h = view_max_w / aspect;
    }

    if (view_max_w < view_new_w) {
        view_new_w = view_max_w;
        view_new_h = view_max_h;
    }

    k__window.view_w = view_new_w;
    k__window.view_h = view_new_h;
    k__window.view_window_ratio = k__window.view_w / (float)k__window.window_w;

    k__mouse.x_at_view = (float)k__mouse.x_at_window * k__window.view_window_ratio;
    k__mouse.y_at_view = (float)k__mouse.y_at_window * k__window.view_window_ratio;
    k__mouse.x_at_room = k__mouse.x_at_view + k__window.view_x;
    k__mouse.y_at_room = k__mouse.y_at_view + k__window.view_y;

    return 0;
}

int k_view_set_position(float cx, float cy) {

    if (isnan(cx) || isinf(cx) || isnan(cy) || isinf(cy)) {
        k_log_error("invalid parameters `cx`=%f, `cy`=%f", cx, cy);
        return -1;
    }

    struct k_room *room = k__current_room;

    float view_half_w = k__window.view_w / 2;

    if (cx <= view_half_w) {
        k__window.view_x = 0;
    } else if (room->room_w - view_half_w <= cx) {
        k__window.view_x = room->room_w - k__window.view_w;
    } else {
        k__window.view_x = cx - view_half_w;
    }

    float view_half_h = k__window.view_h / 2;

    if (cy <= view_half_h) {
        k__window.view_y = 0;
    } else if (room->room_h - view_half_h <= cy) {
        k__window.view_y = room->room_h - k__window.view_h;
    } else {
        k__window.view_y = cy - view_half_h;
    }

    k__mouse.x_at_room = k__mouse.x_at_view + k__window.view_x;
    k__mouse.y_at_room = k__mouse.y_at_view + k__window.view_y;

    return 0;
}

void k_view_get_rect(float *get_x, float *get_y, float *get_w, float *get_h) {
    if (NULL != get_x) { *get_x = k__window.view_x; }
    if (NULL != get_y) { *get_y = k__window.view_y; }
    if (NULL != get_w) { *get_w = k__window.view_w; }
    if (NULL != get_h) { *get_h = k__window.view_h; }
}

void k_view_get_position(float *get_cx, float *get_cy) {
    if (NULL != get_cx) { *get_cx = k__window.view_x + k__window.view_w / 2; }
    if (NULL != get_cy) { *get_cy = k__window.view_y + k__window.view_h / 2; }
}
