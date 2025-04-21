#ifndef K_MOUSE_H
#define K_MOUSE_H

float k_mouse_x(void);

float k_mouse_y(void);

float k_mouse_window_x(void);

float k_mouse_window_y(void);

enum k_mouse_button {
    K_BUTTON_LEFT   = 0,
    K_BUTTON_MIDDLE = 1,
    K_BUTTON_RIGHT  = 2,
};

/* 按下按键 */
int k_mouse_button_pressed(enum k_mouse_button button);

/* 松开按键 */
int k_mouse_button_released(enum k_mouse_button button);

/* 按住按键 */
int k_mouse_button_held(enum k_mouse_button button);

/* 按键空闲 */
int k_mouse_button_idle(enum k_mouse_button button);

/* 按下或按住 */
int k_mouse_button_down(enum k_mouse_button button);

/* 没有按下，也没有按住 */
int k_mouse_button_up(enum k_mouse_button button);

#endif
