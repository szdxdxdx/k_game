#include <math.h>

#include "k_game.h"

#include "../k_SDL/_shared.h"
#include "../room/_shared.h"

#include "./_shared.h"

struct k_view_context k__view = {
    .view_x = 0,
    .view_y = 0,
    .view_w = 0,
    .view_h = 0,
};

void k_view_set_w(float w) {

    if (w <= 0)
        return;

    struct k_room *room = K_CURRENT_ROOM;

    float max_w = fminf(room->room_w, (float)k__SDL.canvas_w);
    float max_h = fminf(room->room_h, (float)k__SDL.canvas_h);
    float aspect = (float)k__SDL.window_w / (float)k__SDL.window_h;

    float new_w = fminf(w, max_w);
    float new_h = new_w / aspect;

    if (new_h <= max_h) {
        new_h = max_h;
        new_w = max_h * aspect;
    }

    k__view.view_w = new_w;
    k__view.view_h = new_h;
}

void k_view_set_h(float h) {

    if (h <= 0)
        return;

    struct k_room *room = K_CURRENT_ROOM;

    float max_w = fminf(room->room_w, (float)k__SDL.canvas_w);
    float max_h = fminf(room->room_h, (float)k__SDL.canvas_h);
    float aspect = (float)k__SDL.window_w / (float)k__SDL.window_h;

    float new_h = fminf(h, max_h);
    float new_w = new_h * aspect;

    if (new_w <= max_w) {
        new_w = max_w;
        new_h = max_w / aspect;
    }

    k__view.view_w = new_w;
    k__view.view_h = new_h;
}

void k_view_set_position(float cx, float cy) {

    struct k_room *room = K_CURRENT_ROOM;

    float half_w = k__view.view_w / 2;

    if (cx <= half_w) {
        k__view.view_x = 0;
    }
    else if (room->room_w - half_w <= cx) {
        k__view.view_x = room->room_w - k__view.view_w;
    }
    else {
        k__view.view_x = cx - half_w;
    }

    float half_h = k__view.view_h / 2;

    if (cy <= half_h) {
        k__view.view_y = 0;
    }
    else if (room->room_h - half_h <= cy) {
        k__view.view_y = room->room_h - k__view.view_h;
    }
    else {
        k__view.view_y = cy - half_h;
    }
}
