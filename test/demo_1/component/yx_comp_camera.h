#ifndef YX_COMP_CAMERA_H
#define YX_COMP_CAMERA_H

#include "k_game.h"

int yx_camera_component_define(void);

void yx_room_add_camera_manager(void);

int yx_object_add_camera_follow(struct k_object *object, float *x, float *y, float weight);

#endif
