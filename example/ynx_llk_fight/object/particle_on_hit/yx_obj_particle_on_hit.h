
#ifndef YX_OBJ_PARTICLE_ON_HIT_H
#define YX_OBJ_PARTICLE_ON_HIT_H

#include "k_game.h"

struct yx_obj_particle_on_hit {
    float x;
    float y;
};

struct yx_obj_particle_on_hit *yx_obj_particle_on_hit_create(float x, float y);

#endif
