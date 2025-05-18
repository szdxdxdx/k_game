#ifndef YX_FLOAT_VEC_H
#define YX_FLOAT_VEC_H

#include <math.h>

struct yx_float_vec2 {
    float x;
    float y;
};

#define yx_float_vec2_zero ((struct yx_float_vec2) { 0.0f, 0.0f })

/* 加 */
static inline struct yx_float_vec2 yx_float_vec2_add(struct yx_float_vec2 v1, struct yx_float_vec2 v2) {
    struct yx_float_vec2 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    return result;
}

/* 减 */
static inline struct yx_float_vec2 yx_float_vec2_sub(struct yx_float_vec2 v1, struct yx_float_vec2 v2) {
    struct yx_float_vec2 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    return result;
}

/* 反向 */
static inline struct yx_float_vec2 yx_float_vec2_neg(struct yx_float_vec2 v) {
    struct yx_float_vec2 result;
    result.x = -v.x;
    result.y = -v.y;
    return result;
}

/* 缩放（数乘） */
static inline struct yx_float_vec2 yx_float_vec2_scale(struct yx_float_vec2 v, float k) {
    struct yx_float_vec2 result;
    result.x = v.x * k;
    result.y = v.y * k;
    return result;
}

/* 内积（点乘） */
static inline float yx_float_vec2_dot(struct yx_float_vec2 v1, struct yx_float_vec2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

/* 长度 */
static inline float yx_float_vec2_length(struct yx_float_vec2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

/* 长度的平方 */
static inline float yx_float_vec2_length_squared(struct yx_float_vec2 v) {
    return v.x * v.x + v.y * v.y;
}

/* 归一化 */
static inline struct yx_float_vec2 yx_float_vec2_normalize(struct yx_float_vec2 v) {
    struct yx_float_vec2 result;
    if (v.x == 0.0f && v.y == 0.0f) {
        result.x = 0.0f;
        result.y = 0.0f;
    }
    else {
        float len = sqrtf(v.x * v.x + v.y * v.y);
        result.x = v.x / len;
        result.y = v.y / len;
    }
    return result;
}

/* 右旋转 90 度 */
static inline struct yx_float_vec2 yx_float_vec2_perp_right(struct yx_float_vec2 v) {
    struct yx_float_vec2 result;
    result.x = v.y;
    result.y = -v.x;
    return result;
}

/* 线性插值 */
static inline struct yx_float_vec2 yx_float_vec2_lerp(struct yx_float_vec2 v1, struct yx_float_vec2 v2, float t) {
    struct yx_float_vec2 result;
    result.x = v1.x + t * (v2.x - v1.x);
    result.y = v1.y + t * (v2.y - v1.y);
    return result;
}

#endif
