#ifndef K__CAMERA_INTERNAL_H
#define K__CAMERA_INTERNAL_H

#include <stddef.h>

#include "k_game/core/k_object.h"
#include "k_game/core/k_component.h"
#include "k_game/core/k_callback.h"

#include "k_game/ext/k_camera.h"

struct k_camera_target;
struct k_camera;

extern struct k_component_type *k__camera_component_type;

/* region [camera_target] */

struct k_camera_target {
    struct k_component *component;

    float *x;
    float *y;

    float weight;
};

int k__camera_target_init(struct k_component *component, void *params);

void k__camera_target_fini(struct k_component *component);

/* endregion */

/* region [camera] */

enum k_camera_state {
    K__CAMERA_DISABLE,
    K__CAMERA_AUTO_FOLLOW,
    K__CAMERA_CINEMATIC,
};

#define K__CAMERA_TARGET_MAX 16

struct k_camera {

    enum k_camera_state state;

    float acceleration;
    float max_speed;
    float vx;
    float vy;

    struct k_camera_target *primary_target;
    struct k_camera_target *targets[K__CAMERA_TARGET_MAX];
    size_t targets_num;

    struct k_callback *cb_camera_move;
};

void k__camera_update(void *camera_);

/* endregion */

#endif
