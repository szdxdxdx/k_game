#include "k_log.h"

#include "k_game/alloc.h"

#include "k_game/component.h"
#include "./k_component_type.h"
#include "./k_component_registry.h"

static int check_config(const struct k_component_type_config *config) {
    const char *err_msg = "";

    if (NULL == config) {
        err_msg = "assert( NULL != config )";
        goto err;
    }

    if (NULL == config->fn_create) {
        err_msg = "assert( NULL != fn_create )";
        goto err;
    }

    return 0;

err:
    k_log_error("Invalid component type config: %s", err_msg);
    return -1;
}

struct k_component_type *k_define_component_type(const struct k_component_type_config *config) {

    if (0 != check_config(config))
        goto err;

    struct k_component_type *component_type = k_malloc(sizeof(struct k_component_type));
    if (NULL == component_type)
        goto err;

    if (0 != k__component_registry_add(component_type, config->type_name)) {
        k_free(component_type);
        goto err;
    }

    component_type->data_size  = config->data_size;
    component_type->fn_create  = config->fn_create;
    component_type->fn_destroy = config->fn_destroy;

    return component_type;

err:
    k_log_error("Failed to define component type");
    return NULL;
}

void k__undef_component_type(struct k_component_type *component_type) {
    k__component_registry_del(component_type);
    k_free(component_type);
}
