#include "k_str_map.h"

#define K_LOG_TAG "k_game:room_blackboard"
#include "k_log.h"

#include "k_game/core/k_component.h"

#include "k_game/ext/k_room_blackboard.h"
#include "./k_room_blackboard_register.h"
#include "k_game/core/k_mem_alloc.h"

static struct k_component_type *k__room_blackboard_component_type;

struct k_room_blackboard {
    struct k_str_map map
};

void *k_room_blackboard_add(const char *key, size_t value_size) {

    struct k_room_blackboard *blackboard = k_room_get_component_manager_data(k__room_blackboard_component_type);
    if (NULL == blackboard) {
        k_log_error("current room doesn't have blackboard. please add a blackboard to the room first");
        return NULL;
    }

    return k_str_map_add(&blackboard->map, key, value_size);
}

void *k_room_blackboard_put(const char *key, size_t value_size) {

    struct k_room_blackboard *blackboard = k_room_get_component_manager_data(k__room_blackboard_component_type);
    if (NULL == blackboard) {
        k_log_error("current room doesn't have blackboard. please add a blackboard to the room first");
        return NULL;
    }

    return k_str_map_put(&blackboard->map, key, value_size);
}

void *k_room_blackboard_get(const char *key) {

    struct k_room_blackboard *blackboard = k_room_get_component_manager_data(k__room_blackboard_component_type);
    if (NULL == blackboard) {
        k_log_error("current room doesn't have blackboard. please add a blackboard to the room first");
        return NULL;
    }

    return k_str_map_get(&blackboard->map, key);
}

void k_room_blackboard_del(const char *key) {

    struct k_room_blackboard *blackboard = k_room_get_component_manager_data(k__room_blackboard_component_type);
    if (NULL == blackboard) {
        k_log_error("current room doesn't have blackboard. please add a blackboard to the room first");
        return;
    }

    k_str_map_remove(&blackboard->map, key);
}

static int k__room_blackboard_on_create(struct k_component_manager *manager, void *param) {
    (void)param;

    struct k_room_blackboard *blackboard = k_component_manager_get_data(manager);

    struct k_str_map_options options;
    options.fn_malloc = k__mem_alloc;
    options.fn_free   = k__mem_free;
    if (NULL == k_str_map_construct(&blackboard->map, &options))
        return -1;

    return 0;
}

static void k__room_blackboard_on_destroy(struct k_component_manager *manager) {
    struct k_room_blackboard *blackboard = k_component_manager_get_data(manager);
    k_str_map_destruct(&blackboard->map);
}

/* region [component_type_register] */

int k__component_type_register_room_blackboard(void) {

    struct k_component_manager_config manager_config = K_COMPONENT_MANAGER_CONFIG_INIT;
    manager_config.data_size = sizeof(struct k_room_blackboard);
    manager_config.on_create = k__room_blackboard_on_create;
    manager_config.on_destroy = k__room_blackboard_on_destroy;

    struct k_component_type *type = k_component_type_register(&manager_config, NULL);
    if (NULL == type)
        return -1;

    k__room_blackboard_component_type = type;
    return 0;
}

/* endregion */

int k_room_add_blackboard(void) {
    return k_room_add_component_manager(k__room_blackboard_component_type, NULL);
}
