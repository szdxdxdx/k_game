#include "./k_collision_internal.h"

int k__collision_manager_init(struct k_component_manager *component_manager, void *param) {
    (void)param;

    struct k_collision_manager *manager = k_component_manager_get_data(component_manager);

    if (0 != k__collision_manager_init_group_map(manager))
        return -1;

    manager->component_manager = component_manager;
    return 0;
}

void k__collision_manager_fini(struct k_component_manager *component_manager) {
    struct k_collision_manager *manager = k_component_manager_get_data(component_manager);

    k__collision_manager_fini_group_map(manager);
}
