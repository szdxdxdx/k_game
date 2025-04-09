#ifndef K__CALLBACK_BASE_H
#define K__CALLBACK_BASE_H

#include <stdint.h>

#include "k_list.h"

struct k_room;
struct k_object;
struct k_component;
struct k_component_manager;

enum k_callback_context {

    K__ROOM_CALLBACK,
    K__OBJECT_CALLBACK,
    K__COMPONENT_CALLBACK,

    /* experimental */
    K__COMPONENT_MANAGER_CALLBACK,
};

enum k_callback_event {

    K__ALARM_CALLBACK,
    K__STEP_CALLBACK,
    K__DRAW_CALLBACK,
};

enum k_callback_state {

    /* 新创建的回调节点存储在预备容器中，处于【未激活】状态 */
    K__CALLBACK_INACTIVE,

    /* 刷新预备容器后，节点变为【已激活】状态，并被转移到正式容器中 */
    K__CALLBACK_ACTIVE,

    /* 【计时器回调】执行后，节点变为【已执行】状态 */
    K__CALLBACK_EXECUTED,

    /* 删除回调时，节点被标记为【逻辑删除】并移入预备容器。下次刷新时，释放节点内存 */
    K__CALLBACK_DELETED,
};

struct k_callback {
    uint8_t context : 8;
    uint8_t event   : 8;
    uint8_t state   : 8;

    struct k_list_node context_callback_list_node;
};

#endif
