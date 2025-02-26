#ifndef K_GAME_COMPONENT_H
#define K_GAME_COMPONENT_H

#include <stddef.h>

enum k_event_type { /* <- tmp */
    K_EVENT_STEP,
    K_EVENT_DRAW,
    K_EVENT_ALARM,
};

struct k_object;

struct k_component_type;

struct k_component;

struct k_component_type_config {

    const char *component_type_name;

    size_t data_size;

    void (*fn_create)(struct k_component *component, void *params);

    void (*fn_destroy)(struct k_component *component);

    struct k_component_callback_config {

        size_t callback_storage_offset;

        enum k_event_type callback_type;

        union {
            struct {
                void (*fn_callback)(struct k_component *component);
            } step_callback;

            struct {
                void (*fn_callback)(struct k_component *component);
                int z_index;
            } draw_callback;

            struct {
                void (*fn_callback)(struct k_component *component, int timeout_diff);
                int delay_ms;
            } alarm_callback;
        };
    } *callbacks;

    size_t callbacks_num;
};

struct k_component_type *k_define_component_type(const struct k_component_type_config *config);

/* ------------------------------------------------------------------------ */

struct k_object *k_component_get_object(struct k_component *component);

void *k_component_get_data(struct k_component *component);

/* ------------------------------------------------------------------------ */

struct k_component_callback;

struct k_component_callback *k_component_add_alarm_callback(struct k_component *component, void (*fn_callback(struct k_component *component)), int delay_ms);

struct k_component_callback *k_component_add_step_callback(struct k_component *component, void (*fn_callback(struct k_component *component)));

struct k_component_callback *k_component_add_draw_callback(struct k_component *component, void (*fn_callback(struct k_component *component)), int z_index);

void k_component_del_callback(struct k_component_callback *callback);

/* ------------------------------------------------------------------------ */

// int k_get_objects_with_component(size_t component_type_id, struct k_object ***get_objects, size_t *n);

#endif
