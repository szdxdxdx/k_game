#ifndef K__COMPONENT_MANAGER_MAP_H
#define K__COMPONENT_MANAGER_MAP_H

#include "k_array.h"

struct k_room;
struct k_component_manager_type;
struct k_component_manager;

/* 全局单例容器，用于管理所有的组件管理器实例 */
struct k_room_component_manager_map {

    /* 使用动态二维数组建立从【房间、组件类型】到【组件管理器实例】的索引，
     * 能以 O(1) 的时间复杂度，快速获取特定房间中挂载的特定类型的组件管理器实例。
     *
     * 用类似 C++ 的模板语法来表示 map 的类型应为：
     * `k_array<k_array<k_component_manager *> *> map;`
     */
    struct k_array map;
};

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
