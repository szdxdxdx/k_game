#ifndef K_POSITION_H
#define K_POSITION_H

#include "k_list.h"

#include "../core/k_rect.h"

struct k_position;

struct k_position_config {

    float *world_x;
    float *world_y;

    struct k_position *parent;

    float local_x;
    float local_y;
};

struct k_position *k_object_add_position(struct k_object *object, const struct k_position_config *config);

void k_object_del_position(struct k_position *position);

void k_position_set_local_position(struct k_position *position, float local_x, float local_y);

void k_position_set_world_position(struct k_position *position, float world_x, float world_y);

#endif
