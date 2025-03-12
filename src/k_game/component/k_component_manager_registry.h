#ifndef K_GAME__COMPONENT_MANAGER_REGISTRY_H
#define K_GAME__COMPONENT_MANAGER_REGISTRY_H

struct k_component_manager_registry {

    void *_; /* TODO */
};

void k_component_manager_registry_init(struct k_component_manager_registry *registry);

void k_component_manager_registry_deinit(struct k_component_manager_registry *registry);

#endif
