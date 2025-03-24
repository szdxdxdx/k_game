#ifndef K_MATH_H
#define K_MATH_H

#include <math.h>

/** \brief 计算从点 (ox, oy) 到点 (x, y) 的向量的方向角的余弦和正弦值 */
static inline void calc_vec_cos_and_sin(float ox, float oy, float x, float y, float *get_cos, float *get_sin) {

    if (ox == x) {
        if (oy == y) {
            *get_cos = 1.0f;
            *get_sin = 0.0f;
        }
        else if (oy < y) {
            *get_cos = 0.0f;
            *get_sin = 1.0f;
        }
        else {
            *get_cos = 0.0f;
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
