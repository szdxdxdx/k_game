#ifndef K_GAME__COLLISION__INTERNAL_H
#define K_GAME__COLLISION__INTERNAL_H

#include <stdint.h>

#include "k_list.h"
#include "k_int_map.h"

#include "k_game.h"

#include "./_public.h"

extern struct k_component_type *K__COMPONENT_TYPE_COLLISION;

/* region [config] */

enum k_collision_box_type {
    K_COLLISION_BOX_RECTANGLE,
    K_COLLISION_BOX_CIRCLE,
};

struct k_collision_box_config {
    enum k_collision_box_type type;
    void *config;
};

/* endregion */

/* region [collision_box] */

struct k_collision_rectangle {
    float offset_x1;
    float offset_y1;
    float offset_x2;
    float offset_y2;
};

struct k_collision_circle {
    float offset_cx;
    float offset_cy;
    float r;
};

struct k_collision_box {

    struct k_list_node list_node;

    struct k_component *component;

    struct k_float_vec2 *position;

    enum k_collision_box_type box_type;
    union {
        struct k_collision_rectangle rectangle;
        struct k_collision_circle    circle;
    };
};

int k__collision_box_init(struct k_component *component, void *params);

/* endregion */

/* region [collision_group] */

struct k_collision_group {

    struct k_int_map_node map_node;

    struct k_list box_list;
};

/* endregion */

/* region [collision_manager] */

struct k_collision_manager {
    struct k_int_map group_map;
};

int k__collision_manager_init(struct k_component_manager *component_manager, void *params);

void k__collision_manager_fini(struct k_component_manager *component_manager);

int k__collision_manager_add(struct k_collision_manager *manager, struct k_collision_box *box, int group_idx);

/* endregion */

/* region [debug] */

void k__collision_debug_draw(struct k_component *component);

/* endregion */

#endif
