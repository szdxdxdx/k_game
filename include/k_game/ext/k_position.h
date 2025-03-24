#ifndef K_POSITION_H
#define K_POSITION_H

#include "k_list.h"

#include "../core/k_rect.h"

struct k_position;

struct k_position_config {

    float *x;
    float *y;

    struct k_position *parent;

    float rel_x;
    float rel_y;
};

struct k_position *k_object_add_position(struct k_object *object, const struct k_position_config *config);

void k_object_del_position(struct k_position *position);

void k_position_set(struct k_position *self, float x, float y);

void k_position_set_rel(struct k_position *self, float rel_x, float rel_y);

#endif
