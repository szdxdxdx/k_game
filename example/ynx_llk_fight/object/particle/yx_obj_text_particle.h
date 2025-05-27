#ifndef YX_OBJ_particle_text_on_hit_H
#define YX_OBJ_particle_text_on_hit_H

#include <stdint.h>

struct yx_obj_particle_text_on_hit_config {
    float x;
    float y;
    float vx;
    float vy;
};

void yx_obj_particle_text_on_hit_create(const struct yx_obj_particle_text_on_hit_config *config, const char *fmt, ...);

struct yx_obj_particle_text_on_hit {
    char *text;
    uint32_t color;
    float x;
    float y;
    float vx;
    float vy;
};

#endif
