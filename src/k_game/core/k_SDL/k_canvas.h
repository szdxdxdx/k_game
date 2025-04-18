#ifndef K__CANVAS_H
#define K__CANVAS_H

#include "SDL_render.h"

enum k_canvas_viewport {
    K__CANVAS_VIEWPORT_NONE,
    K__CANVAS_VIEWPORT_ROOM,
    K__CANVAS_VIEWPORT_UI,
};

struct k_canvas {

    SDL_Texture *canvas;

    /* 将画布分为左右两个区域，左边绘制房间，右边绘制 ui */

    SDL_FRect room_viewport;
    SDL_FRect ui_viewport;

    enum k_canvas_viewport current_viewport;

    SDL_FRect viewport_rect;

    float viewport_w;
    float viewport_h;
};

extern struct k_canvas k__canvas;

void k__canvas_present(void);

#endif
