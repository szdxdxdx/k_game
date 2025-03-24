#include "k_seq_step.h"
#include "k_log.h"

#include "./_internal.h"

static struct k_asset_registry component_type_registry;

/* region [component_define] */

/* region [steps] */

struct step_context {
    const struct k_component_manager_config *manager_config;
    const struct k_component_entity_config *entity_config;
    struct k_component_type *component_type;
};

static int step_check_config(void *context) {
    struct step_context *ctx = context;
    const struct k_component_entity_config *entity_config = ctx->entity_config;

    const char *err_msg;

#define check_config_assert(cond) \
    do { if ( ! (cond)) { err_msg = "assert( " #cond " )"; goto err; }} while(0)

    check_config_assert(NULL != entity_config);
    check_config_assert(NULL != entity_config->fn_init);

    return 0;

err:
    k_log_error("Invalid component config: %s", err_msg);
    return -1;
}

static int step_malloc(void *context) {
    struct step_context *ctx = context;

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

    if (NULL != ctx->manager_config) {
        ctx->component_type = k_malloc(sizeof(struct k_component_type) + sizeof(struct k_component_manager_type));
        if (NULL == ctx->component_type) {
            return -1;
        } else {
            ctx->component_type->manager_type = ptr_offset(ctx->component_type, sizeof(struct k_component_type));
            return 0;
        }
    } else {
        ctx->component_type = k_malloc(sizeof(struct k_component_type));
        if (NULL == ctx->component_type) {
            return -1;
        } else {
            ctx->component_type->manager_type = NULL;
            return 0;
        }
    }
}

static void step_free(void *context) {
    struct step_context *ctx = context;
    struct k_component_type *component_type = ctx->component_type;

    k_free(component_type);
}

static size_t id_counter = 0;

static int step_set_properties(void *context) {
    struct step_context *ctx = context;
    struct k_component_type *component_type = ctx->component_type;

    struct k_component_entity_type *entity_type = &component_type->entity_type;
    const struct k_component_entity_config *entity_config = ctx->entity_config;
    entity_type->data_size = entity_config->data_size;
    entity_type->fn_init   = entity_config->fn_init;
    entity_type->fn_fini   = entity_config->fn_fini;

    if (NULL != component_type->manager_type) {

        struct k_component_manager_type *manager_type = component_type->manager_type;
        const struct k_component_manager_config *manager_config = ctx->manager_config;
        manager_type->type_id   = id_counter++;
        manager_type->data_size = manager_config->data_size;
        manager_type->fn_init   = manager_config->fn_init;
        manager_type->fn_fini   = manager_config->fn_fini;
    }

    return 0;
}

static int step_registry_add(void *context) {
    struct step_context *ctx = context;
    struct k_component_type *component_type = ctx->component_type;

    k__asset_registry_add(&component_type_registry, &component_type->registry_node);
    return 0;
}

static void step_registry_del(void *context) {
    struct step_context *ctx = context;
    struct k_component_type *component_type = ctx->component_type;

    k__asset_registry_del(&component_type->registry_node);
}

static struct k_seq_step steps[] = {
    { step_check_config,   NULL              },
    { step_malloc,         step_free         },
    { step_set_properties, NULL              },
    { step_registry_add,   step_registry_del },
};

/* endregion */

struct k_component_type *k_component_define(const struct k_component_manager_config *manager_config, const struct k_component_entity_config *entity_config) {

    struct step_context ctx;
    ctx.manager_config = manager_config;
    ctx.entity_config  = entity_config;
    ctx.component_type = NULL;

    if (0 != k_seq_step_exec(steps, k_seq_step_array_len(steps), &ctx)) {
        k_log_error("Failed to define component type");
        return NULL;
    }

    return ctx.component_type;
}

static void k__component_undef(struct k_component_type *component_type) {

    struct step_context ctx;
    ctx.manager_config = NULL;
    ctx.entity_config  = NULL;
    ctx.component_type = component_type;

    k_seq_step_exec_backward(steps, k_seq_step_array_len(steps), &ctx);
}

/* endregion */

/* region [component_type_registry] */

int k__init_component_type_registry(void) {
    return k__asset_registry_init(&component_type_registry);
}

static void fn_release_asset(struct k_asset_registry_node *registry_node) {
    struct k_component_type *component_type = container_of(registry_node, struct k_component_type, registry_node);
    k__component_undef(component_type);
}

void k__cleanup_component_type_registry(void) {
    k__asset_registry_cleanup(&component_type_registry, fn_release_asset);
}

int k_component_type_set_name(struct k_component_type *component_type, const char *type_name) {
    return k__asset_set_name(&component_type_registry, &component_type->registry_node, type_name);
}

struct k_component_type *k_find_component_type(const char *type_name) {

    struct k_asset_registry_node *registry_node = k__asset_registry_find(&component_type_registry, type_name);
    if (NULL == registry_node)
        return NULL;

    struct k_component_type *component_type = container_of(registry_node, struct k_component_type, registry_node);
    return component_type;
}

/* endregion */
