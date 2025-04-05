#include <math.h>
#include "./_internal.h"

#include "../k_SDL/_shared.h"
#include "../game/_shared.h"

void k_room_set_view_w(struct k_room *room, float w) {

    if (w <= 0)
        return;

    float max_w = fminf(room->room_w, (float)k__SDL.canvas_w);
    float max_h = fminf(room->room_h, (float)k__SDL.canvas_h);
    float aspect = (float)k__SDL.window_w / (float)k__SDL.window_h;

    float new_w = fminf(w, max_w);
    float new_h = new_w / aspect;

    if (new_h <= max_h) {
        new_h = max_h;
        new_w = max_h * aspect;
    }

    room->view_w = new_w;
    room->view_h = new_h;
}

void k_room_set_view_h(struct k_room *room, float h) {

    if (h <= 0)
        return;

    float max_w = fminf(room->room_w, (float)k__SDL.canvas_w);
    float max_h = fminf(room->room_h, (float)k__SDL.canvas_h);
    float aspect = (float)k__SDL.window_w / (float)k__SDL.window_h;

    float new_h = fminf(h, max_h);
    float new_w = new_h * aspect;

    if (new_w <= max_w) {
        new_w = max_w;
        new_h = max_w / aspect;
    }

    room->view_w = new_w;
    room->view_h = new_h;
}

void k_room_set_view_x(struct k_room *room, float x) {

    if (x <= 0) {
        room->view_x = 0;
        return;
    }

    float max_x = room->room_w - room->view_w;
    if (max_x <= x) {
        room->view_x = max_x;
        return;
    }

    room->view_x = x;
}

void k_room_set_view_y(struct k_room *room, float y) {

    if (y <= 0) {
        room->view_y = 0;
        return;
    }

    float max_y = room->room_h - room->view_h;
    if (max_y <= y) {
        room->view_y = max_y;
        return;
    }

    room->view_y = y;
}

void k_room_set_view_cx(struct k_room *room, float cx) {

    float half_w = room->view_w / 2;

    if (cx <= half_w) {
        room->view_x = 0;
    }
    else if (room->room_w - half_w <= cx) {
        room->view_x = room->room_w - room->view_w;
    }
    else {
        room->view_x = cx - half_w;
    }
}

void k_room_set_view_cy(struct k_room *room, float cy) {

    float half_h = room->view_h / 2;

    if (cy <= half_h) {
        room->view_y = 0;
    }
    else if (room->room_h - half_h <= cy) {
        room->view_y = room->room_h - room->view_h;
    }
    else {
        room->view_y = cy - half_h;
    }
}
