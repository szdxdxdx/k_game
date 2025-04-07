#include <math.h>

#include "k_game/core/k_view.h"
#include "./k_window.h"
#include "./k_mouse.h"

#include "../room/k_room.h"

void k_view_fit_rect(float w, float h) {

    if (w <= 0 || h <= 0)
        return;

    float view_max_w = fminf(k__current_room->room_w, (float)k__window.canvas_w);
    float view_max_h = fminf(k__current_room->room_h, (float)k__window.canvas_h);

    float view_new_w = fminf(w, view_max_w);
    float view_new_h = view_new_w / k__window.window_aspect_ratio;

    if (view_max_h < view_new_h) {
        view_new_w = view_max_h * k__window.window_aspect_ratio;
        view_new_h = view_max_h;
    }
    else if (view_new_h < h) {
        view_new_h = h;
        view_new_w = view_new_h * k__window.window_aspect_ratio;

        if (view_max_h < view_new_h) {
            view_new_w = view_max_h * k__window.window_aspect_ratio;
            view_new_h = view_max_h;
        }
    }

    k__window.view_w = view_new_w;
    k__window.view_h = view_new_h;
    k__window.view_window_ratio = k__window.view_w / (float)k__window.window_w;

    k__mouse.x_at_view = (float)k__mouse.x_at_window * k__window.view_window_ratio;
    k__mouse.y_at_view = (float)k__mouse.y_at_window * k__window.view_window_ratio;
    k__mouse.x_at_room = k__mouse.x_at_view + k__window.view_x;
    k__mouse.y_at_room = k__mouse.y_at_view + k__window.view_y;
}

void k_view_set_position(float cx, float cy) {

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
}
