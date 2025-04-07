#ifndef K__COMPONENT_MANAGER_MAP_H
#define K__COMPONENT_MANAGER_MAP_H

struct k_room;
struct k_component_manager_type;
struct k_component_manager;

int k__component_manager_map_init(void);

void k__component_manager_map_deinit(void);

int k__component_manager_map_add(struct k_room *room, struct k_component_manager *manager);

void k__component_manager_map_del(struct k_room *room, struct k_component_manager_type *manager_type);

struct k_component_manager *k__component_manager_map_find(struct k_room *room, struct k_component_manager_type *manager_type);

/**
 * \brief 删除房间中挂载的的所有组件管理器【暂未公布的 API】
 *
 * 未公布本 API 的原因参考 `k__room_del_component_manager()`，
 * 本函数目前仅用在房间销毁时执行清理操作。
 */
void k__room_del_all_component_managers(struct k_room *room);

#endif
