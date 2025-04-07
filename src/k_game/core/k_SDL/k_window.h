#ifndef K__WINDOW_H
#define K__WINDOW_H

#include "SDL_render.h"

struct k_window_context {

    /* 窗口 */
    SDL_Window *window;
    int window_w;
    int window_h;

    /* 渲染器 */
    SDL_Renderer *renderer;

    /* 画布 */
    SDL_Texture *canvas;
    int canvas_w;
    int canvas_h;

    /* 视野 */
    float view_h;
    float view_w;
    float view_x;
    float view_y;
};

extern struct k_window_context k__window;

#endif
