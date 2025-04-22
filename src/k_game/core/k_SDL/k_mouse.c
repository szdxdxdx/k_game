#include <assert.h>

#include "k_game/core/k_mouse.h"
#include "./k_mouse.h"
#include "./k_view.h"

struct k_mouse_context k__mouse;

/* region [handle_SDL_event] */

void k__SDL_handle_event_mouse_button_down(SDL_MouseButtonEvent *event) {

    switch (event->button) {
        case SDL_BUTTON_LEFT:
            k__mouse.button_state[K_BUTTON_LEFT] |= 0b010;
            break;
        case SDL_BUTTON_MIDDLE:
            k__mouse.button_state[K_BUTTON_MIDDLE] |= 0b010;
            break;
        case SDL_BUTTON_RIGHT:
            k__mouse.button_state[K_BUTTON_RIGHT] |= 0b010;
            break;
        default:
            assert(0);
    }
}

void k__SDL_handle_event_mouse_button_up(SDL_MouseButtonEvent *event) {

    switch (event->button) {
        case SDL_BUTTON_LEFT:
            k__mouse.button_state[K_BUTTON_LEFT] |= 0b001;
            break;
        case SDL_BUTTON_MIDDLE:
            k__mouse.button_state[K_BUTTON_MIDDLE] |= 0b001;
            break;
        case SDL_BUTTON_RIGHT:
            k__mouse.button_state[K_BUTTON_RIGHT] |= 0b001;
            break;
        default:
            assert(0);
    }
}

void k__SDL_handle_event_mouse_motion(struct SDL_MouseMotionEvent *event) {

    k__mouse.window_x = (float)event->x;
    k__mouse.view_x   = k__mouse.window_x * k__view.view_window_ratio;
    k__mouse.room_x   = k__mouse.view_x + k__view.view_x;

    k__mouse.window_y = (float)event->y;
    k__mouse.view_y   = k__mouse.window_y * k__view.view_window_ratio;
    k__mouse.room_y   = k__mouse.view_y + k__view.view_y;
}

/* endregion */

/* region [position] */

float k_mouse_x(void) {
    return k__mouse.room_x;
}

float k_mouse_y(void) {
    return k__mouse.room_y;
}

float k_mouse_window_x(void) {
    return k__mouse.window_x;
}

float k_mouse_window_y(void) {
    return k__mouse.window_y;
}

/* endregion */

/* region [button_state] */

static void refresh_button_state(enum k_mouse_button button) {

    switch (k__mouse.button_state[button] & 0b11) {
        case 0b00: k__mouse.button_state[button] &= 0b100; break;
        case 0b01: k__mouse.button_state[button]  = 0b000; break;
        case 0b10: k__mouse.button_state[button]  = 0b100; break;
        case 0b11: k__mouse.button_state[button]  = 0b000; break;
    }
}

void k__mouse_refresh_state(void) {
    refresh_button_state(K_BUTTON_LEFT);
    refresh_button_state(K_BUTTON_MIDDLE);
    refresh_button_state(K_BUTTON_RIGHT);
}

int k_mouse_button_pressed(enum k_mouse_button button) {
    return 0b010 == (k__mouse.button_state[button] & 0b110);
}

int k_mouse_button_released(enum k_mouse_button button) {
    return 0b001 == (k__mouse.button_state[button] & 0b001);
}

int k_mouse_button_held(enum k_mouse_button button) {
    return 0b100 == (k__mouse.button_state[button] & 0b101);
}

int k_mouse_button_idle(enum k_mouse_button button) {
    return 0b000 == k__mouse.button_state[button];
}

int k_mouse_button_down(enum k_mouse_button button) {
    return 0b010 == (k__mouse.button_state[button] & 0b110)
        || 0b100 == (k__mouse.button_state[button] & 0b101);
}

int k_mouse_button_up(enum k_mouse_button button) {
    return 0b010 != (k__mouse.button_state[button] & 0b110)
        && 0b100 != (k__mouse.button_state[button] & 0b101);
}

/* endregion */
