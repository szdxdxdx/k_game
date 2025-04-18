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

    SDL_FRect room_viewport;
    SDL_FRect ui_viewport;
    enum k_canvas_viewport current_viewport;

    SDL_FRect clip_rect;
};

extern struct k_canvas k__canvas;

void k__canvas_present(void);

#endif
