#ifndef K_TIME_H
#define K_TIME_H

#include <stdint.h>

/** \brief 获取上一帧到当前帧所经过的时间，单位：秒 */
float k_time_get_step_delta(void);

/** \brief 获取上一帧到当前帧所经过的时间，单位：毫秒 */
int k_time_get_step_delta_ms(void);

/** \brief 获取当前帧的时间戳，单位：毫秒 */
uint64_t k_time_get_step_timestamp(void);

/** \brief 获取当前时间戳，单位：毫秒 */
uint64_t k_time_get_timestamp(void);

#endif
