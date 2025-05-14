#ifndef K__TIME_H
#define K__TIME_H

#include <stdint.h>

struct k_time_context {

    /* 记录当前帧的时间戳，单位：毫秒 */
    uint64_t step_timestamp;

    /* 记录两帧时间差，单位：毫秒 */
    int step_delta_ms;

    /* 记录两帧时间差，单位：秒 */
    float step_delta;
};

extern struct k_time_context k__time;

#endif
