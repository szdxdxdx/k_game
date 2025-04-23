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

/** \brief 检测鼠标按键是否在当前帧内被按下 */
int k_mouse_button_down(enum k_mouse_button button);

/** \brief 检测鼠标按键是否持续被按住 */
int k_mouse_button_held(enum k_mouse_button button);

/** \brief 检测鼠标按键是否在当前帧被按下或按住 */
int k_mouse_button_down_or_held(enum k_mouse_button button);

/** \brief 松开鼠标按键是否在当前帧内被松开 */
int k_mouse_button_up(enum k_mouse_button button);

/** \brief 检测鼠标按键是否处于空闲状态 */
int k_mouse_button_idle(enum k_mouse_button button);

#endif
