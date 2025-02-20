#ifndef K_GAME__GAME_CONTEXT_H
#define K_GAME__GAME_CONTEXT_H

#include <stdint.h>

struct k_room;

struct k_game_context {

    unsigned int quit_game;

    /** \brief 记录两帧间的时间差，单位：毫秒 */
    int delta_ms;

    /** \brief 记录游戏启动以来经过的时间，单位：毫秒 */
    uint64_t current_ms;

    struct k_room *current_room;
};

extern struct k_game_context k__game;

#endif
