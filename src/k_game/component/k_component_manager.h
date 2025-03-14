#ifndef K_GAME__COMPONENT_MANAGER_H
#define K_GAME__COMPONENT_MANAGER_H

struct k_component_manager;

/* region [component_manager_map] */

int k__component_manager_map_init(void);

void k__component_manager_map_free(void);

struct k_component_manager *k__component_manager_map_find(struct k_room *room, struct k_component_manager_type *manager_type);

/* endregion */

/* region [component_manager] */

struct k_component_manager {

    void *data;

    struct k_component_type *component_type;

    struct k_room *room;
};

void k__room_del_all_component_managers(struct k_room *room);

/* endregion */

#endif
