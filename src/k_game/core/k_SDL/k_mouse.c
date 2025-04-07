#include "./_internal.h"

#include "./k_window.h"

static int mouse_x = 0;
static int mouse_y = 0;
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

void k__SDL_refresh_mouse_state(void) {
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


    float x_at_window = (float)event->x;
    float x_at_view = (x_at_window / (float)k__window.window_w) * k__window.view_w;
    float x_at_room = x_at_view + k__window.view_x;
    mouse_x = (int)x_at_room;

    float y_at_window = (float)event->y;
    float y_at_view = (y_at_window / (float)k__window.window_h) * k__window.view_h;
    float y_at_room = y_at_view + k__window.view_y;
    mouse_y = (int)y_at_room;
}

int k_mouse_x(void) {
    return mouse_x;
}

int k_mouse_y(void) {
    return mouse_y;
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
