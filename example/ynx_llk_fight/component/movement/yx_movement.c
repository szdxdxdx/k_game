
#include "k_game.h"

#include "yx_movement.h"


static int yx_movement_on_create(struct k_component *component, void *param) {


    return 0;
}

int yx_component_type_register_movement(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size = sizeof(struct yx_movement);
    config.on_create = yx_movement_on_create;

    struct k_component_type *type = k_component_type_register(NULL, &config);
    if (NULL == type)
        return -1;

    return 0;
}
