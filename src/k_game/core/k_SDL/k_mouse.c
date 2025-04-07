#include "k_game/core/k_mouse.h"
#include "./k_mouse.h"
#include "./k_window.h"

static int mouse_x_at_window = 0;
static int mouse_y_at_window = 0;
static float mouse_x_at_view = 0;
static float mouse_y_at_view = 0;
static float mouse_x_at_room = 0;
static float mouse_y_at_room = 0;

static uint8_t button_state[3] = { 0 };

static void refresh_button_state(enum k_mouse_button button) {

    /* 每个按键用 3 个 bit 记录状态：
     * - 0b100 表示该按键在上一帧被按下或按住
     * - 0b010 表示该按键在这一帧被按下
     * - 0b001 表示该按键在这一帧抬起
     */
    switch (button_state[button] & 0b11) {
        case 0b00: button_state[button] &= 0b100; break;
        case 0b01: button_state[button]  = 0b000; break;
        case 0b10: button_state[button]  = 0b100; break;
        case 0b11: button_state[button]  = 0b000; break;
    }
}

void k__mouse_refresh_state(void) {
    refresh_button_state(K_BUTTON_LEFT);
    refresh_button_state(K_BUTTON_MIDDLE);
    refresh_button_state(K_BUTTON_RIGHT);
}

void k__SDL_handle_event_mouse_button_down(SDL_MouseButtonEvent *event) {
    switch (event->button) {
        case SDL_BUTTON_LEFT:   button_state[K_BUTTON_LEFT]   |= 0b010; break;
        case SDL_BUTTON_MIDDLE: button_state[K_BUTTON_MIDDLE] |= 0b010; break;
        case SDL_BUTTON_RIGHT:  button_state[K_BUTTON_RIGHT]  |= 0b010; break;
    }
}

void k__SDL_handle_event_mouse_button_up(SDL_MouseButtonEvent *event) {
    switch (event->button) {
        case SDL_BUTTON_LEFT:   button_state[K_BUTTON_LEFT]   |= 0b001; break;
        case SDL_BUTTON_MIDDLE: button_state[K_BUTTON_MIDDLE] |= 0b001; break;
        case SDL_BUTTON_RIGHT:  button_state[K_BUTTON_RIGHT]  |= 0b001; break;
    }
}

void k__SDL_handle_event_mouse_motion(struct SDL_MouseMotionEvent *event) {

    mouse_x_at_window = event->x;
    mouse_x_at_view   = (float)mouse_x_at_window * k__window.view_window_ratio;
    mouse_x_at_room   = mouse_x_at_view + k__window.view_x;

    mouse_y_at_window = event->y;
    mouse_y_at_view   = (float)mouse_y_at_window * k__window.view_window_ratio;
    mouse_y_at_room   = mouse_y_at_view + k__window.view_y;
}

void k__mouse_update_cursor_position_after_view_change(void) {
    mouse_x_at_room = mouse_x_at_view + k__window.view_x;
    mouse_y_at_room = mouse_y_at_view + k__window.view_y;
}

int k_mouse_x(void) {
    return (int)mouse_x_at_room;
}

int k_mouse_y(void) {
    return (int)mouse_y_at_room;
}

int k_mouse_button_pressed(enum k_mouse_button button) {
    return 0b010 == (button_state[button] & 0b110);
}

int k_mouse_button_released(enum k_mouse_button button) {
    return 0b001 == (button_state[button] & 0b001);
}

int k_mouse_button_held(enum k_mouse_button button) {
    return 0b100 == (button_state[button] & 0b101);
}

int k_mouse_button_idle(enum k_mouse_button button) {
    return 0b000 == button_state[button];
}

int k_mouse_button_down(enum k_mouse_button button) {
    return 0b010 == (button_state[button] & 0b110)
        || 0b100 == (button_state[button] & 0b101);
}

int k_mouse_button_up(enum k_mouse_button button) {
    return 0b010 != (button_state[button] & 0b110)
        && 0b100 != (button_state[button] & 0b101);
}
