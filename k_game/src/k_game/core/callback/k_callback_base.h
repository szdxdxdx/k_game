#ifndef K__CALLBACK_BASE_H
#define K__CALLBACK_BASE_H

#include <stdint.h>

#include "k_list.h"

#include "k_game/core/k_game_fwd.h"

struct k_callback {

    struct k_list_node context_callback_list_node;

    void (*fn_del)(struct k_callback *callback);

    /* 曾经的设计方案：通过 context 和 event 两个枚举字段来区分回调的事件类型和上下文
     *
     * size_t context;
     * size_t event;
     */
};

void k__room_del_all_callbacks(struct k_room *room);

void k__object_del_all_callbacks(struct k_object *object);

void k__component_del_all_callbacks(struct k_component *component);

void k__component_manager_del_all_callbacks(struct k_component_manager *manager);

#endif
