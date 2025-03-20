#ifndef K_GAME__COMPONENT__INTERNAL_H
#define K_GAME__COMPONENT__INTERNAL_H

#include "k_game.h"

#include "./_public.h"

/* region [component_manager] */

struct k_component_manager {

    void *data;

    struct k_component_type *component_type;

    struct k_room *room; /* [?] 不是必须的 */
};

/**
 * \brief 删除房间中的组件管理器【暂未公布的 API】
 *
 * 当前情况是，删除组件管理器后，组件实例所持有的管理器指针将悬空。
 * 我不确定有没有必要更新组件实例，将该指针置为 NULL？
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

/* region [component_manager_map] */

struct k_component_manager *k__component_manager_map_find(struct k_room *room, struct k_component_manager_type *manager_type);

/* endregion */

#endif
