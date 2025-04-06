#ifndef K_GAME_CONTEXT_H
#define K_GAME_CONTEXT_H

#include "SDL_render.h"

struct k_room;

struct k_game_context {

    unsigned int quit_game;

    /* 记录当前帧的时间戳，单位：毫秒 */
    uint64_t step_timestamp;

    /* 记录两帧时间差，单位：毫秒 */
    int step_delta_ms;

    /* 记录两帧时间差，单位：秒 */
    float step_delta;

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

    /* 当前房间 */
    struct k_room *current_room;
};

extern struct k_game_context k__game;

#endif
