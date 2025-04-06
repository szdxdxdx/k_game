#ifndef K_GAME_SHARED_H
#define K_GAME_SHARED_H

#include <stdint.h>

#include "SDL_render.h"
#include "SDL_events.h"

struct k_game_context {

    unsigned int quit_game;

    /* 记录当前帧的时间戳，单位：毫秒 */
    uint64_t step_timestamp;

    /* 记录两帧时间差，单位：毫秒 */
    int step_delta_ms;

    /* 记录两帧时间差，单位：秒 */
    float step_delta;

    SDL_Window *window;
    int window_w;
    int window_h;

    SDL_Renderer *renderer;

    SDL_Texture *canvas;
    int canvas_w;
    int canvas_h;

    float view_h;
    float view_w;
    float view_x;
    float view_y;
};

extern struct k_game_context k__game;

#endif
