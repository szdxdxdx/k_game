
#define K_LOG_TAG "k_game:component"
#include "k_log.h"

#include "k_game/core/k_mem_alloc.h"

#include "k_game/core/k_component.h"
#include "./k_component_type.h"
#include "./k_component_type_registry.h"

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

static int check_config(const struct k_component_manager_config *manager_config, const struct k_component_entity_config *entity_config) {

    if (NULL != entity_config) {
        if (NULL == entity_config->on_create) {
            k_log_error("`entity_config->on_create` is null");
            return -1;
        }
    }

    if (NULL != manager_config) {
        if (NULL == manager_config->on_create) {
            k_log_error("`manager_config->on_create` is null");
            return -1;
        }
    }

    return 0;
}

static int id_counter = 0;

static int k__component_on_create_default(struct k_component *component, void *param) {
    (void)component;
    (void)param;
    return 0;
}

struct k_component_type *k_component_type_register(const struct k_component_manager_config *manager_config, const struct k_component_entity_config *entity_config) {

    if (0 != check_config(manager_config, entity_config))
        goto err;

    struct k_component_type *component_type;

    if (NULL != manager_config) {
        component_type = k__mem_alloc(sizeof(struct k_component_type) + sizeof(struct k_component_manager_type));
        if (NULL == component_type)
            goto err;

        component_type->manager_type = ptr_offset(component_type, sizeof(struct k_component_type));
    }
    else {
        component_type = k__mem_alloc(sizeof(struct k_component_type));
        if (NULL == component_type)
            goto err;

        component_type->manager_type = NULL;
    }

    struct k_component_entity_type *entity_type = &component_type->entity_type;
    if (NULL != entity_config) {
        entity_type->data_size  = entity_config->data_size;
        entity_type->on_create  = entity_config->on_create;
        entity_type->on_destroy = entity_config->on_destroy;
    } else {
        entity_type->data_size  = 0;
        entity_type->on_create  = k__component_on_create_default;
        entity_type->on_destroy = NULL;
    }

    struct k_component_manager_type *manager_type = component_type->manager_type;
    if (NULL != manager_type) {
        manager_type->type_id    = id_counter++;
        manager_type->data_size  = manager_config->data_size;
        manager_type->on_create  = manager_config->on_create;
        manager_type->on_destroy = manager_config->on_destroy;
    }

    k__component_type_registry_add(component_type);

    return component_type;

err:
    k_log_error("Failed to define component type");
    return NULL;
}

void k__component_type_unregister(struct k_component_type *component_type) {

    k__component_type_registry_del(component_type);
    k__mem_free(component_type);
}
