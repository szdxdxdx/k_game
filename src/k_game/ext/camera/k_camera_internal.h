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

int k__camera_target_on_create(struct k_component *component, void *param);

void k__camera_target_on_destroy(struct k_component *component);

/* endregion */

/* region [camera] */

enum k_camera_state {
    K__CAMERA_DISABLE,
    K__CAMERA_AUTO_FOLLOW,
    K__CAMERA_CINEMATIC,
};

#define K__CAMERA_TARGET_MAX 16

struct k_camera {
    struct k_component_manager *component_manager;

    enum k_camera_state state;

    float acceleration;
    float max_speed;
    float vx;
    float vy;
    float dst_x;
    float dst_y;

    struct k_camera_target *primary_target;
    struct k_camera_target *targets[K__CAMERA_TARGET_MAX];
    size_t targets_num;

    struct k_callback *cb_camera_move;

    /* ------------------------------------------------------------------------ */

    struct k_callback *cb_camera_debug;

    /* ------------------------------------------------------------------------ */

    void *webui;
};

void k__camera_move_on_begin_step(void *camera_);

int k__camera_set_debug_draw_enabled(struct k_camera *camera, int enabled);

void k__camera_webui(struct k_camera *camera, int enabled);

/* endregion */

#endif
