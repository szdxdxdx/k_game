#include "k_log.h"

#include "k_game_alloc.h"
#include "./component.h"

static int check_config(const struct k_component_type_config *config) {
    const char *err_msg;

#define check_config_assert(cond) \
    do { if ( ! (cond)) { err_msg = "assert( " #cond " )"; goto err; }} while(0)

    check_config_assert(NULL != config);
    check_config_assert(NULL != config->fn_init);

    return 0;

err:
    k_log_error("Invalid component type config: %s", err_msg);
    return -1;
}

struct k_component_type *k_component_define(const struct k_component_type_config *config) {

    if (0 != check_config(config))
        goto err;

    struct k_component_type *component_type = k_malloc(sizeof(struct k_component_type));
    if (NULL == component_type)
        goto err;

    if (0 != k__component_registry_add(component_type, config->type_name)) {
        k_free(component_type);
        goto err;
    }

    component_type->data_size = config->data_size;
    component_type->fn_init = config->fn_init;
    component_type->fn_fini = config->fn_fini;

    return component_type;

err:
    k_log_error("Failed to define component type");
    return NULL;
}

void k__component_undef(struct k_component_type *component_type) {
    k__component_registry_del(component_type);
    k_free(component_type);
}
