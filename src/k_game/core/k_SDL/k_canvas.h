#ifndef K__CANVAS_H
#define K__CANVAS_H

#include "SDL_render.h"

enum k_canvas_target {
    K__CANVAS_TARGET_NONE,
    K__CANVAS_TARGET_ROOM,
    K__CANVAS_TARGET_UI,
};

struct k_canvas {

    /* 画布 */
    SDL_Texture *canvas;

    enum k_canvas_target canvas_target;

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

extern struct k_canvas k__canvas;

void k__canvas_set_target(enum k_canvas_target target);

#endif
