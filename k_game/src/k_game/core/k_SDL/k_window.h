#ifndef K__WINDOW_H
#define K__WINDOW_H

#include "SDL_render.h"

struct k_window_context {

    /* 窗口 */
    SDL_Window *window;

    /* 保存一份 float 类型的窗口宽高值 */
    float window_w;
    float window_h;

    /* 记录窗口的长宽比，改变窗口宽高时同步修改该值 */
    float window_aspect_ratio;

    /* 渲染器 */
    SDL_Renderer *renderer;
};

extern struct k_window_context k__window;

#endif
