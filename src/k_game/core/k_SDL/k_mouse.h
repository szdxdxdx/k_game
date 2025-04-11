#ifndef K__MOUSE_H
#define K__MOUSE_H

#include "SDL_events.h"

struct k_mouse_context {

    /* 鼠标在游戏窗口中的位置 */
    int window_x;
    int window_y;

    /* 鼠标在视野中的位置 */
    float view_x;
    float view_y;

    /* 鼠标在房间中的位置 */
    float room_x;
    float room_y;

    /* 记录鼠标按键状态
     *
     * 每个按键用 3 个 bit 记录状态：
     * - 0b100 表示该按键在上一帧被按下或按住
     * - 0b010 表示该按键在这一帧被按下
     * - 0b001 表示该按键在这一帧抬起
     */
    uint8_t button_state[3];
};

extern struct k_mouse_context k__mouse;

void k__mouse_refresh_state(void);

void k__SDL_handle_event_mouse_button_down(SDL_MouseButtonEvent *event);

void k__SDL_handle_event_mouse_button_up(SDL_MouseButtonEvent *event);

void k__SDL_handle_event_mouse_motion(SDL_MouseMotionEvent *event);

#endif
