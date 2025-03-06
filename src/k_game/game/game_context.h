#ifndef K_GAME__GAME_CONTEXT_H
#define K_GAME__GAME_CONTEXT_H

#include <stdint.h>

struct k_room;

struct k_game_context {

    unsigned int quit_game;

    /* 记录当前帧的时间戳，单位：毫秒 */
    uint64_t step_timestamp;

    /* 记录两帧时间差，单位：毫秒 */
    int step_delta_ms;

    /* 记录两帧时间差，单位：秒 */
    float step_delta;

    struct k_room *current_room;
};

extern struct k_game_context k__game;

#endif
