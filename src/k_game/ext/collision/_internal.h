#ifndef K_COLLISION_INTERNAL_H
#define K_COLLISION_INTERNAL_H

#include "k_list.h"
#include "k_int_map.h"

#include "k_game.h"

#include "./_public.h"

enum k_collision_box_type;
struct k_collision_box_config;

struct k_collision_box;
struct k_collision_rectangle;
struct k_collision_circle;

struct k_collision_group;
struct k_collision_manager;

extern struct k_component_type *k__component_type_collision;

/* region [config] */

enum k_collision_box_type {

    K_COLLISION_POINT,
    K_COLLISION_LINE,
    K_COLLISION_RECTANGLE,
    K_COLLISION_CIRCLE,
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

    struct k_list_node box_list_node;

    struct k_component *component;

    /* group 也支持 debug，如果 group 开启了 debug，新增 box 时也自动启用 debug */
    struct k_component_callback *cb_debug_draw;

    float *x;
    float *y;

    enum k_collision_box_type type;

    union {
        struct k_collision_point     point;
        struct k_collision_line      line;
        struct k_collision_rectangle rect;
        struct k_collision_circle    circle;
    };
};

int k__collision_box_init(struct k_component *component, void *params);

void k__collision_box_fini(struct k_component *component);

/* endregion */

/* region [collision_group] */

struct k_collision_group {

    struct k_int_map_node group_map_node;

    struct k_list box_list;
};

struct k_collision_group *k__collision_manager_find_or_add_group(struct k_collision_manager *manager, int group_id);

void k__collision_manager_del_group(struct k_collision_group *group);

/* endregion */

/* region [collision_manager] */

struct k_collision_manager {

    struct k_int_map group_map;
};

int k__collision_manager_init(struct k_component_manager *component_manager, void *params);

void k__collision_manager_fini(struct k_component_manager *component_manager);

int k__collision_manager_add_box(struct k_collision_manager *manager, struct k_collision_box *box, int group_id);

/* endregion */

/* region [debug] */

void k__collision_debug_draw(struct k_component *component);

/* endregion */

#endif
