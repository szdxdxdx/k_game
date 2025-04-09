#ifndef K__COMPONENT_MANAGER_H
#define K__COMPONENT_MANAGER_H

#include <stddef.h>

#include "k_list.h"

struct k_component_type;
struct k_room;

struct k_component_manager {

    void *data;

    struct k_component_type *component_type;

    struct k_room *room;

    struct k_list callback_list;
};

void k__component_manager_destroy(struct k_component_manager *manager);

/**
 * \brief 删除房间中的组件管理器【暂未公布的 API】
 *
 * 当前情况是，删除组件管理器后，组件实例所持有的管理器指针将悬空。
 * 我不确定有没有必要更新组件实例，将该指针置为 `NULL`？
 */
void k__room_del_component_manager(struct k_room *room, struct k_component_type *component_type);

#endif
