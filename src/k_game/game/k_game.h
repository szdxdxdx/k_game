#ifndef K_GAME__GAME_H
#define K_GAME__GAME_H

#include <stdint.h>

#include "SDL.h"

#include "k_game/game.h"

struct k_room;

struct k_game_context {

    SDL_Window *window;

    SDL_Renderer *renderer;

    unsigned int quit_game;

    /** \brief 记录两帧间的时间差，单位：秒 */
    float delta_time;

    /** \brief 记录两帧间的时间差，单位：毫秒 */
    int delta_ms;

    /** \brief 记录游戏启动以来经过的时间，单位：毫秒 */
    uint64_t current_ms;

    struct k_room *current_room;
};

extern struct k_game_context k_game;

int k__game_SDL_init(const struct k_game_config *config);

void k__game_SDL_deinit(void);

#endif
