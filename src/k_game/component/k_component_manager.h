#ifndef K_GAME__COMPONENT_MANAGER_H
#define K_GAME__COMPONENT_MANAGER_H

struct k_component_manager {

    void *data;

    struct k_component_type *component_type;

    struct k_room *room;
};

void k__component_manager_destroy_self(struct k_component_manager *manager);

#endif
