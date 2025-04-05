#ifndef K_GAME_PUBLIC_H
#define K_GAME_PUBLIC_H

#include <stdint.h>

struct k_game_config;
struct k_room;

struct k_game_context {

    unsigned int quit_game;

    /* 记录当前帧的时间戳，单位：毫秒 */
    uint64_t step_timestamp;

    /* 记录两帧时间差，单位：毫秒 */
    int step_delta_ms;

    /* 记录两帧时间差，单位：秒 */
    float step_delta;
};

extern struct k_game_context k__game;

#endif
