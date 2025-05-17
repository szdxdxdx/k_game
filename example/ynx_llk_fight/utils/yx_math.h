#ifndef YX_MATH_H
#define YX_MATH_H

#include <math.h>
#include <stdlib.h>

/** \brief 归一化，将 [a, b] 区间内的一个值 t 映射成 [0, 1] 区间内的值 */
static inline float yx_normalized(float a, float b, float t) {
    return (t - a) / (b - a);
}

/** \brief 判断点是否在矩形内 */
static inline int yx_point_in_rect(float x, float y, struct k_float_rect *rect) {
    return x >= rect->x && x <= rect->x + rect->w && y >= rect->y && y <= rect->y + rect->h;
}

/**
 * \brief 计算向量方向角的余弦值和正弦值
 *
 * `(ox, oy)` 和 `(x, y)` 分别为向量的起点坐标和终点坐标。
 * 出参 `get_cos` 和 `get_sin` 返回向量余弦值和正弦值。
 * 零向量的方向定义为 x 轴正方向。
 */
static inline void yx_calc_vector_direction(float ox, float oy, float x, float y, float *get_cos, float *get_sin) {

    if (ox == x) {
        if (oy < y) {
            *get_cos = 0.0f;
            *get_sin = 1.0f;
        }
        else if (oy == y) {
            *get_cos = 1.0f;
            *get_sin = 0.0f;
        }
        else {
            *get_cos =  0.0f;
            *get_sin = -1.0f;
        }
    }
    else {
        float angle = atanf((y - oy) / (x - ox));
        if (ox < x) {
            *get_cos = cosf(angle);
            *get_sin = sinf(angle);
        } else {
            *get_cos = -cosf(angle);
            *get_sin = -sinf(angle);
        }
    }
}

static inline float yx_rand(float min, float max) {
    int min_ = (int)min;
    int max_ = (int)max;
    int r = min_ + rand() % (max_ - min_ + 1);
    return (float)r;
}

#endif
