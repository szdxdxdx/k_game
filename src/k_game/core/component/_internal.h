#ifndef K_COMPONENT_INTERNAL_H
#define K_COMPONENT_INTERNAL_H

#include "k_game.h"

#include "./_public.h"

/* region [component_manager] */

struct k_component_manager {

    void *data;

    struct k_component_type *component_type;

    struct k_room *room; /* [?] 不是必须的 */
};

/* endregion */

/* region [component_manager_map] */

struct k_component_manager *k__component_manager_map_find(struct k_room *room, struct k_component_manager_type *manager_type);

/* endregion */

#endif
