#ifndef K_GAME_OBJECT_H
#define K_GAME_OBJECT_H

#include <stddef.h>

struct k_object;

struct k_object *k_create_object(size_t data_size);

void k_destroy_object(struct k_object *object);

void *k_object_get_data(struct k_object *object);

/* ------------------------------------------------------------------------ */

struct k_object_callback;

struct k_object_callback *k_object_add_step_begin_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_object_callback *k_object_add_alarm_callback(struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms);

struct k_object_callback *k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_object_callback *k_object_add_draw_callback(struct k_object *object, void (*fn_callback)(struct k_object *object), int z_index);

struct k_object_callback *k_object_add_step_end_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

void k_object_del_callback(struct k_object_callback *callback);

/* ------------------------------------------------------------------------ */

struct k_component_type;
struct k_component;

struct k_component *k_object_add_component(struct k_object *object, struct k_component_type *component_type, void *params);

void k_object_del_component(struct k_object *object, struct k_component *component);

struct k_component *k_object_get_component(struct k_object *object, struct k_component_type *component_type);

void *k_object_get_component_data(struct k_object *object, struct k_component_type *component_type);

#endif
