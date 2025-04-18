#ifndef K__WINDOW_H
#define K__WINDOW_H

#include "SDL_render.h"

struct k_window_context {

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
};

extern struct k_window_context k__window;

#endif
