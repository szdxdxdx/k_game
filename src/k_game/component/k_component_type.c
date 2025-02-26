#include "k_log.h"

#include "k_game/component.h"
#include "./k_component_type.h"
#include "k_game/alloc.h"

static int check_config(const struct k_component_type_config *config) {

    if (NULL == config) {
        k_log_error("Invalid component type config: assert( NULL != config )");
        return -1;
    }

    if (NULL == config->fn_create) {
        k_log_error("Invalid component type config: assert( NULL != fn_create )");
        return -1;
    }

    return 0;
}

struct k_component_type *k_define_component_type(const struct k_component_type_config *config) {

    if (0 != check_config(config))
        return NULL;

    struct k_component_type *component_type = k_malloc(sizeof(struct k_component_type));
    if (NULL != component_type)
        return NULL;

    if (0 != k__component_registry_add(&component_type->registry_node, config->component_type_name)) {
        k_free(component_type);
        return NULL;
    }

    component_type->data_size = config->data_size;
    component_type->fn_create = config->fn_create;

    return component_type;
}
