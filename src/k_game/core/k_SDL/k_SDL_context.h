#ifndef K_SDL_CONTEXT_H
#define K_SDL_CONTEXT_H

#include "SDL_render.h"

struct k_SDL_context {

    /* 窗口 */
    SDL_Window *window;

    /* 框架中多是浮点运算，保存一份 float 类型的宽高值 */
    float window_w;
    float window_h;

    /* 记录窗口的长宽比，改变窗口宽高时同步修改该值 */
    float window_aspect_ratio;

    /* 渲染器 */
    SDL_Renderer *renderer;

    /* ------------------------------------------------------------------------ */

    /* 视野左上角在房间中的坐标 */
    float view_x;
    float view_y;

    /* 视野的宽高 */
    float view_h;
    float view_w;

    /* 记录视野与窗口的缩放比，改变视野宽高或窗口宽高时同步修改该值 */
    float view_window_ratio;

    /* ------------------------------------------------------------------------ */

    /* 记录当前帧的时间戳，单位：毫秒 */
    uint64_t time_step_timestamp;

    /* 记录两帧时间差，单位：毫秒 */
    int time_step_delta_ms;

    /* 记录两帧时间差，单位：秒 */
    float time_step_delta;

    /* ------------------------------------------------------------------------ */

    /* 鼠标在游戏窗口中的位置 */
    float mouse_window_x;
    float mouse_window_y;

    /* 鼠标在视野中的位置 */
    float mouse_view_x;
    float mouse_view_y;

    /* 鼠标在房间中的位置 */
    float mouse_room_x;
    float mouse_room_y;

    /* 记录鼠标按键状态
     *
     * 每个按键用 3 个 bit 记录状态：
     * - 0b100 表示该按键在上一帧被按下或按住
     * - 0b010 表示该按键在这一帧被按下
     * - 0b001 表示该按键在这一帧抬起
     */
    uint8_t mouse_button_state[3];

    /* ------------------------------------------------------------------------ */

    uint8_t key_state[SDL_NUM_SCANCODES];
};

#endif
