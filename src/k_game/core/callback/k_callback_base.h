#ifndef K_GAME__CALLBACK_BASE_H
#define K_GAME__CALLBACK_BASE_H

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

    /* 创建回调时，将新回调的状态设为 PENDING，并把它们放入预备容器 */
    K_CALLBACK_PENDING,

    /* 执行刷新后，预备容器里的回调将被转移到正式容器，此时它们的状态为 ACTIVE */
    K_CALLBACK_ACTIVE,

    /* 计时器回调执行一次后便失效，此时回调状态变为 EXECUTED */
    K_CALLBACK_EXECUTED,

    /* 此状态枚举已弃用，删除的回调统一使用 DEAD 标识
     *
     * 原本的作用是：
     * 手动删除回调后，回调被标记为 DELETED，表示逻辑删除
     * 区分销毁对象或组件时的连带删除
     */
    K_CALLBACK_DELETED,

    /* 销毁对象或组件时，连同删除其绑定的回调，此时回调标记为 DEAD */
    K_CALLBACK_DEAD,
};

struct k_callback_base {
    uint8_t context;
    uint8_t event;
    uint8_t state;
};

void k__callback_flag_deleted(struct k_callback_base *callback); /* <- 弃用 */

void k__callback_flag_dead(struct k_callback_base *callback);

#endif
