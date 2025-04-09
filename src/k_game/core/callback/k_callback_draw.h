#ifndef K__CALLBACK_DRAW_H
#define K__CALLBACK_DRAW_H

#include "k_list.h"
#include "k_int_hash_map.h"

#include "./k_callback_base.h"

struct k_draw_callback_manager {

    struct k_list group_list;
    struct k_int_hash_map group_map;

    struct k_list callback_pending_list;
};

int k__draw_callback_manager_init(struct k_draw_callback_manager *manager);

void k__draw_callback_manager_deinit(struct k_draw_callback_manager *manager);

void k__draw_callback_manager_flush(struct k_draw_callback_manager *manager);

void k__draw_callback_manager_exec(struct k_draw_callback_manager *manager);

struct k_callback *k__draw_callback_manager_add_room_callback(struct k_draw_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data), int z_group, int z_layer);

struct k_callback *k__draw_callback_manager_add_object_callback(struct k_draw_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object), int z_group, int z_layer);

struct k_callback *k__draw_callback_manager_add_component_callback(struct k_draw_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component), int z_group, int z_layer);

/* experimental */
struct k_callback *k__draw_callback_manager_add_component_manager_callback(struct k_draw_callback_manager *manager, struct k_component_manager *component_manager, void (*fn_callback)(struct k_component_manager *component_manager), int z_group, int z_layer);

void k__draw_callback_manager_del_callback(struct k_callback *callback);

#endif
