#ifndef K_GAME_COMPONENT_H
#define K_GAME_COMPONENT_H

#include <stddef.h>

enum k_event_type { /* <- tmp */
    K_EVENT_STEP,
};

struct k_object;

struct k_component_callback {

    enum k_event_type events; /* <- tmp */

    void (*fn_callbacks)(void *component_data, struct k_object *object);
};

struct k_component_type_config {

    const char *component_type_name;

    size_t data_size;

    void (*fn_create)(void *params, void *component_data, struct k_object *object);

    void (*fn_destroy)(void *component_data, struct k_object *object);

    struct k_component_callback *callbacks;

    size_t callbacks_num;
};

size_t k_define_component_type(const struct k_component_type_config *config);

int k_get_objects_with_component(size_t component_type_id, struct k_object ***get_objects, size_t *n);

#endif
