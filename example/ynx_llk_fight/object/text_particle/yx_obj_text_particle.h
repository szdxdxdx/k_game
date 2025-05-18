#ifndef YX_OBJ_TEXT_PARTICLE_H
#define YX_OBJ_TEXT_PARTICLE_H

#include <stdint.h>

struct yx_obj_text_particle_config {
    float x;
    float y;
    float vx;
    float vy;
    uint32_t color;
};

void yx_obj_text_particle_create(const struct yx_obj_text_particle_config *config, const char *fmt, ...);

struct yx_obj_text_particle {
    char *text;
    uint32_t color;
    float x;
    float y;
    float vx;
    float vy;
};

#endif
