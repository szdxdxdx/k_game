#ifndef YX_MATH_H
#define YX_MATH_H

#include <math.h>

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

#endif
