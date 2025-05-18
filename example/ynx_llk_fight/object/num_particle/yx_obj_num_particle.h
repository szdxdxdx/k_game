#ifndef YX_OBJ_NUM_PARTICLE_H
#define YX_OBJ_NUM_PARTICLE_H

#include <stdint.h>

void yx_obj_num_particle_create(float x, float y, int num, uint32_t color);

struct yx_obj_num_particle {
    int num;
    uint32_t color;
    float x;
    float y;
    float vx;
    float vy;
};

#endif
