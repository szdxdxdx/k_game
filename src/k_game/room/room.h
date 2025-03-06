#ifndef K_GAME__ROOM_H
#define K_GAME__ROOM_H

#include <stdint.h>

#include "k_game_room.h"

#include "../asset/asset_registry.h"
#include "../object/object_pool.h"

#include "./room_callback_enter.h"
#include "./room_callback_leave.h"
#include "./room_callback_step_begin.h"
#include "./room_callback_step.h"
#include "./room_callback_step_end.h"
#include "./room_callback_alarm.h"
#include "./room_callback_draw.h"

/* region [room_registry] */

struct k_room;

int k__room_registry_init(void);

void k__room_registry_cleanup(void);

int k__room_registry_add(struct k_room *room, const char *room_name);

void k__room_registry_del(struct k_room *room);

/* endregion */

/* region [room] */

struct k_object_pool;

struct k_room {

    struct k_asset_registry_node registry_node;

    struct k_room_enter_callback_registry enter_callbacks;

    struct k_room_leave_callback_registry leave_callbacks;

    struct k_room_step_begin_callback_registry step_begin_callbacks;

    struct k_room_alarm_callback_storage alarm_callbacks;

    struct k_room_step_callback_registry step_callbacks;

    struct k_room_draw_callback_storage draw_callbacks;

    struct k_room_step_end_callback_registry step_end_callbacks;

    struct k_object_pool object_pool;

    int room_w, room_h;

    int (*fn_init)(struct k_room *room, void *params);

    void (*fn_cleanup)(struct k_room *room);

    uint64_t step_interval_ms;

    unsigned int game_loop;

    void *data;
};

void k__room_destroy(struct k_room *room);

/**
 * \brief 销毁房间【此 API 暂未公布】
 *
 * 销毁房间是一件很危险的事，
 * 在游戏运行过程中途销毁房间可能导致不可预期的错误。
 * 建议将房间交由 k_game 在游戏结束时统一销毁。
 */
void k_room_destroy(struct k_room *room);

void k__room_run(struct k_room *room);

/* endregion */

/* region [room_goto] */

void k__room_stack_init(void);

void k__room_stack_cleanup(void);

struct k_room *k__room_stack_get_top(void);

/* endregion */

#endif
