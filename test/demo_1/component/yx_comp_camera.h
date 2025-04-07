#ifndef YX_COMP_CAMERA_H
#define YX_COMP_CAMERA_H

#include "k_game.h"

struct yx_camera_target {

    struct k_component *component;

    size_t target_id;

    float *x;
    float *y;
};

struct yx_camera_manager {

    struct yx_camera_target *targets;
    size_t targets_num;
};

int yx_camera_component_define(void);

#endif
