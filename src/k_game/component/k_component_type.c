#include "k_seq_step.h"
#include "k_log.h"

#include "k_game_alloc.h"

#include "./k_component.h"

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

        /* 组件实体类型的定义和管理器类型的定义结构体共处同一个大内存块 */
        ctx->component_type = k_malloc(sizeof(struct k_component_type) + sizeof(struct k_component_manager_type));
        if (NULL == ctx->component_type) {
            return -1;
        } else {
            ctx->component_type->manager_type = ptr_offset(ctx->component_type, sizeof(struct k_component_type));
            return 0;
        }
    }
    else {
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
    const struct k_component_manager_config *manager_config = ctx->manager_config;
    const struct k_component_entity_config *entity_config = ctx->entity_config;
    struct k_component_type *component_type = ctx->component_type;

    component_type->entity_type.data_size = entity_config->data_size;
    component_type->entity_type.fn_init = entity_config->fn_init;
    component_type->entity_type.fn_fini = entity_config->fn_fini;

    if (NULL != component_type->manager_type) {
        component_type->manager_type->data_size = manager_config->data_size;
        component_type->manager_type->type_id = id_counter++;
    }

    return 0;
}

static int step_registry_add(void *context) {
    struct step_context *ctx = context;
    struct k_component_type *component_type = ctx->component_type;

    k__component_type_registry_add(component_type);
    return 0;
}

static void step_registry_del(void *context) {
    struct step_context *ctx = context;
    struct k_component_type *component_type = ctx->component_type;

    k__component_type_registry_del(component_type);
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

void k__component_undef(struct k_component_type *component_type) {

    struct step_context ctx;
    ctx.manager_config = NULL;
    ctx.entity_config  = NULL;
    ctx.component_type = component_type;

    k_seq_step_exec_backward(steps, k_seq_step_array_len(steps), &ctx);
}
