#ifndef K_COMPONENT_PUBLIC_H
#define K_COMPONENT_PUBLIC_H

#include <stddef.h>

#include "../asset/k_asset_registry.h"

struct k_object;
struct k_component_type;
struct k_component_manager;
struct k_component;

/* region [component_type_registry] */

int k__init_component_type_registry(void);

void k__cleanup_component_type_registry(void);

/* endregion */

/* region [component_type] */

struct k_component_entity_type {

    size_t data_size;

    int (*fn_init)(struct k_component *component, void *params);

    void (*fn_fini)(struct k_component *component);
};

struct k_component_manager_type {

    size_t type_id;

    size_t data_size;

    int (*fn_init)(struct k_component_manager *manager, void *params);

    void (*fn_fini)(struct k_component_manager *manager);
};

struct k_component_type {

    struct k_asset_registry_node registry_node;

    struct k_component_entity_type entity_type;

    struct k_component_manager_type *manager_type;
};

/* endregion */

/* region [component_manager_map] */

int k__init_component_manager_map(void);

void k__deinit_component_manager_map(void);

/* endregion */

/* region [component] */

struct k_component {

    struct k_list_node list_node;

    struct k_component_type *type;

    struct k_object *object;

    struct k_component_manager *manager;

    struct k_list callback_list;

    void *data;
};

/* endregion */

/* region [component_manager] */

/**
 * \brief 删除房间中的组件管理器【暂未公布的 API】
 *
 * 当前情况是，删除组件管理器后，组件实例所持有的管理器指针将悬空。
 * 我不确定有没有必要更新组件实例，将该指针置为 `NULL`？
 */
void k__room_del_component_manager(struct k_room *room, struct k_component_type *component_type);

/**
 * \brief 删除房间中挂载的的所有组件管理器【暂未公布的 API】
 *
 * 未公布本 API 的原因参考 `k__room_del_component_manager()`，
 * 本函数目前仅用在房间销毁时执行清理操作。
 */
void k__room_del_all_component_managers(struct k_room *room);

/* endregion */

#endif
