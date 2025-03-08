#ifndef K_GAME__CALLBACK_DRAW_H
#define K_GAME__CALLBACK_DRAW_H

#include "k_array.h"
#include "k_list.h"

#include "./callback_base.h"

#include "../room/room_callback.h"
#include "../object/object_callback.h"
#include "../component/component_callback.h"

struct k_draw_callback_layer {

    struct k_list_node layer_list_node;

    int z_index;

    struct k_list callback_list;
};

struct k_draw_callback {

    struct k_list_node list_node;

    struct k_callback base;

    struct k_draw_callback_layer *layer;
};

struct k_draw_callback_manager {

    struct k_list layer_list;

    struct k_list pending_layer_list;

    struct k_list pending_callback_list;
};

void k__callback_init_draw_manager(struct k_draw_callback_manager *manager);

void k__callback_deinit_draw_manager(struct k_draw_callback_manager *manager);

void k__callback_flush_draw(struct k_draw_callback_manager *manager);

void k__callback_exec_draw(struct k_draw_callback_manager *manager);

struct k_room_callback *k__callback_add_room_draw(struct k_draw_callback_manager *manager, void (*fn_callback)(void *data), void *data, int z_index);

struct k_object_callback *k__callback_add_object_draw(struct k_draw_callback_manager *manager, void (*fn_callback)(struct k_object *object), struct k_object *object, int z_index);

struct k_component_callback *k__callback_add_component_draw(struct k_draw_callback_manager *manager, void (*fn_callback)(struct k_component *component), struct k_component *component, int z_index);

#endif
