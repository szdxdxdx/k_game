#ifndef K__CANVAS_H
#define K__CANVAS_H

#include "SDL_render.h"

enum k_canvas_target {
    K__CANVAS_TARGET_NONE,
    K__CANVAS_TARGET_ROOM,
    K__CANVAS_TARGET_UI,
};

struct k_canvas {

    SDL_Texture *canvas;

    enum k_canvas_target target;

    /* 画布分左上角房间区域和右上角 ui 区域，分别用于绘制房间和 ui */

    /* 画布的房间区域 */
    float room_x;
    float room_y; /* 画布的房间区域左上角应是 (0,0) */
    float room_w;
    float room_h;

    /* 画布的 ui 区域 */
    float ui_x;
    float ui_y;
    float ui_w;
    float ui_h; /* 画布的 ui 区域宽高应是与窗口宽高保持一致 */
};

extern struct k_canvas k__canvas;

#endif
