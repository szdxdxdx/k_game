#ifndef K__CALLBACK_BASE_H
#define K__CALLBACK_BASE_H

#include <stdint.h>

enum k_callback_context {

    K_ROOM_CALLBACK,
    K_OBJECT_CALLBACK,
    K_COMPONENT_CALLBACK,
};

enum k_callback_event {

    K_ALARM_CALLBACK,
    K_STEP_CALLBACK,
    K_DRAW_CALLBACK,
};

enum k_callback_state {

    /* 新创建的回调节点存储在预备容器中，处于【未激活】状态 */
    K_CALLBACK_INACTIVE,

    /* 刷新预备容器后，节点变为【已激活】状态，并被转移到正式容器中 */
    K_CALLBACK_ACTIVE,

    /* 【计时器回调】执行后，节点变为【已执行】状态 */
    K_CALLBACK_EXECUTED,

    /* 删除回调时，节点被标记为【逻辑删除】并移入预备容器。下次刷新时，释放节点内存 */
    K_CALLBACK_DELETED,
};

struct k_callback_base {
    enum k_callback_context context : 8;
    enum k_callback_event   event   : 8;
    enum k_callback_state   state   : 8;
};

void k__callback_del(struct k_callback_base *callback);

#endif
