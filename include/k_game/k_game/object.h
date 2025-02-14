#ifndef K_GAME_OBJECT_H
#define K_GAME_OBJECT_H

#include <stddef.h>

struct k_room;

struct k_object;

struct k_object_config {

    struct k_room *room;

    size_t data_size;

    void (*fn_create)(struct k_object *object);

    void (*fn_destroy)(struct k_object *object);

    void (*fn_step)(struct k_object *object);

    void (*fn_draw)(struct k_object *object);
};

struct k_object *k_create_object(const struct k_object_config *config);

void k_destroy_object(struct k_object_config *object);

void *k_object_get_data(struct k_object *object);

/* ------------------------------------------------------------------------ */

// int k_object_add_component(struct k_object *object, size_t component_id, void *params);

// int k_object_del_component(struct k_object *object, size_t component_id);

// void *k_object_get_component_data(struct k_object *object, size_t component_id);

/* ------------------------------------------------------------------------ */

int k_object_add_step_begin_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

int k_object_add_alarm_callback(struct k_object *object, void (*fn_callback)(struct k_object *object, float timeout), float delay);

int k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

int k_object_add_draw_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

int k_object_add_step_end_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

#endif
