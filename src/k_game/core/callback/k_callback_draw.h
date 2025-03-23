#ifndef K_GAME__CALLBACK_DRAW_H
#define K_GAME__CALLBACK_DRAW_H

#include "k_list.h"
#include "k_int_map.h"

#include "./k_callback_base.h"

struct k_draw_callback_manager {

    struct k_list    group_list;
    struct k_int_map group_map;

    struct k_list callback_pending_list;
};

int k__callback_init_draw_manager(struct k_draw_callback_manager *manager);

void k__callback_deinit_draw_manager(struct k_draw_callback_manager *manager);

void k__callback_flush_draw(struct k_draw_callback_manager *manager);

void k__callback_exec_draw(struct k_draw_callback_manager *manager);

struct k_room_callback *k__callback_add_room_draw(struct k_draw_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data), int z_group, int z_layer);

struct k_object_callback *k__callback_add_object_draw(struct k_draw_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object), int z_group, int z_layer);

struct k_component_callback *k__callback_add_component_draw(struct k_draw_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component), int z_group, int z_layer);

void k__callback_flag_deleted_draw(struct k_callback_base *callback);

void k__callback_del_draw(struct k_callback_base *callback);

#endif
