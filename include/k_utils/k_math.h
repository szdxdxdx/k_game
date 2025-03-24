#ifndef K_MATH_H
#define K_MATH_H

#include <assert.h>
#include <math.h>

static inline void calc_vec_cos_and_sin(float ox, float oy, float x, float y, float *get_cos, float *get_sin) {

    if (ox == x) {
        *get_cos = 1.0f;
        *get_sin = 0.0f;
    }
    else {
        float angle = atanf((y - oy) / (x - ox));
        if (x < ox) {
            *get_cos = -cosf(angle);
            *get_sin = -sinf(angle);
        } else {
            *get_cos = cosf(angle);
            *get_sin = sinf(angle);
        }
    }
}

#endif
