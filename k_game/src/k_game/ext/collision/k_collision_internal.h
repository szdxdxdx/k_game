#ifndef K__COLLISION_INTERNAL_H
#define K__COLLISION_INTERNAL_H

#include "k_list.h"
#include "k_intrusive_map.h"

#include "k_game/core/k_mem_alloc.h"
#include "k_game/core/k_object.h"
#include "k_game/core/k_component.h"
#include "k_game/core/k_callback.h"

#include "k_game/ext/k_collision_box.h"

enum k_collision_box_type;
struct k_collision_box_config;

struct k_collision_box;
struct k_collision_rect;
struct k_collision_circle;

struct k_collision_group;
struct k_collision_manager;

extern struct k_component_type *k__collision_component_type;

/* region [config] */

enum k_collision_box_type {
    K__COLLISION_POINT,
    K__COLLISION_LINE,
    K__COLLISION_RECTANGLE,
    K__COLLISION_CIRCLE,
};

struct k_collision_box_config {

    enum k_collision_box_type type;

    const void *config;
};

/* endregion */

/* region [collision_box] */

struct k_collision_point {
    float offset_x;
    float offset_y;
};

struct k_collision_line {
    float offset_x1;
    float offset_y1;
    float offset_x2;
    float offset_y2;
};

struct k_collision_rect {
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

    struct k_list_node box_list_node;

    struct k_component *component;

    float *x;
    float *y;

    enum k_collision_box_type type;

    union {
        struct k_collision_point  point;
        struct k_collision_line   line;
        struct k_collision_rect   rect;
        struct k_collision_circle circle;
    };
};

int k__collision_box_on_create(struct k_component *component, void *param);

void k__collision_box_on_destroy(struct k_component *component);

/* endregion */

/* region [collision_group] */

struct k_collision_group {

    struct k_int_intrusive_map_node group_map_node;

    struct k_collision_manager *manager;

    struct k_list box_list;

    struct k_callback *cb_debug_draw;
};

int k__collision_manager_init_group_map(struct k_collision_manager *manager);

void k__collision_manager_fini_group_map(struct k_collision_manager *manager);

struct k_collision_group *k__collision_manager_find_or_add_group(struct k_collision_manager *manager, int group_id);

void k__collision_manager_del_group(struct k_collision_group *group);

struct k_collision_group *k__collision_find_or_add_group(int group_id);

struct k_collision_group *k__collision_find_group(int group_id);

/* endregion */

/* region [collision_manager] */

struct k_collision_manager {

    struct k_component_manager *component_manager;

    struct k_int_intrusive_map group_map;
};

int k__collision_manager_on_create(struct k_component_manager *component_manager, void *param);

void k__collision_manager_on_destroy(struct k_component_manager *component_manager);

/* endregion */

#endif
