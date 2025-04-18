#ifndef K__WINDOW_H
#define K__WINDOW_H

#include "SDL_render.h"

#include "k_game/core/k_canvas.h"

struct k_window_context {

    /* 窗口 */
    SDL_Window *window;
    int window_w;
    int window_h;

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

    /* 画布 */
    SDL_Texture *canvas;

    /* 整块画布的大小 */
    int canvas_w;
    int canvas_h;

    /* 画布的房间区域 */
    float canvas_room_x; /* 房间区域的左上角坐标应是 (0, 0) */
    float canvas_room_y;
    float canvas_room_w;
    float canvas_room_h;

    /* 画布的 ui 区域 */
    float canvas_ui_x;
    float canvas_ui_y;
    float canvas_ui_w; /* ui 区域的宽高应是与窗口宽高保持一致 */
    float canvas_ui_h;
};

extern struct k_window_context k__window;

#endif
