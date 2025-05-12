
#define K_LOG_TAG "yx:room:room_1" /* 定义日志输出的标签 */
#include "k_log.h"

#include "k_game.h"
#include "./yx_room_1.h"

#if 0 /* 介绍如何创建房间 */

static void yx_room_1_on_step(void *data) { /* 房间运行时，每帧调用是步事件回调 */
    k_log_info("room_1 on step");
}
static int yx_room_1_on_create(void *param) { /* 创建房间时执行的回调，在此完成房间的初始化 */
    k_log_info("room_1 on create");
    k_room_add_step_callback(NULL, yx_room_1_on_step); /* 给房间添加一个步事件回调 */
    return 0;
}
static void yx_room_1_on_enter(void) { /* 进入房间时执行的回调 */
    k_log_info("room_1 on enter");
}

struct k_room *yx_room_1_create(void) {
    struct k_room_config config = K_ROOM_CONFIG_INIT; /* 默认的初始化配置，可快速填充字段 */
    config.room_w    = 1920; /* 指定房间的宽高 */
    config.room_h    = 1080;
    config.data_size = sizeof(struct yx_room_1); /* 指定房间关联数据的结构体大小 */
    config.on_create = yx_room_1_on_create; /* 指定房间的回调 */
    config.on_enter  = yx_room_1_on_enter;
    struct k_room *room = k_room_create(&config, NULL); /* 创建房间 */
    if (NULL == room) {
        k_log_error("failed to create room_1");
        return NULL;
    }
    return room;
}

#endif

#if 1 /* 介绍按键检测 */

static void yx_room_1_on_step(void *data) {
    if (k_key_down('A')) k_log_info("按键按下");
    if (k_key_up('A'))   k_log_info("按键抬起");
    if (k_key_held('A')) k_log_info("按键按住");
}

static int yx_room_1_on_create(void *param) {
    k_room_add_step_callback(NULL, yx_room_1_on_step);
    return 0;
}

struct k_room *yx_room_1_create(void) {
    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w    = 1920;
    config.room_h    = 1080;
    config.data_size = sizeof(struct yx_room_1);
    config.on_create = yx_room_1_on_create;
    struct k_room *room = k_room_create(&config, NULL);
    if (NULL == room) {
        k_log_error("failed to create room_1");
        return NULL;
    }
    return room;
}

#endif




