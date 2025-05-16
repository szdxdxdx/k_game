#ifndef YX_MOVEMENT_H
#define YX_MOVEMENT_H

struct yx_movement {

    /* 关联的外部坐标，对象当前的位置坐标 */
    float *x;
    float *y;

    /* 速度 */
    float vx;
    float vy;
    float v_max;

    /* 加速度 */
    float ax;
    float ay;
    float a_max;

    /* 阻尼 */
    float damping;
};

struct yx_movement *yx_object_add_movement(struct k_object *object, float *x, float *y);

void yx_object_del_movement(struct yx_movement *m);

void yx_movement_set_v(struct yx_movement *m, float vx, float vy);

void yx_movement_set_v_max(struct yx_movement *m, float v_max);

#endif
