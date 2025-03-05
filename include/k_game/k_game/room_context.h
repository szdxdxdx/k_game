#ifndef K_GAME__ROOM_CONTEXT_H
#define K_GAME__ROOM_CONTEXT_H

#include <stdint.h>

#include "k_game/asset_registry.h"
#include "k_game/room_object_pool.h"
#include "k_game/room_callback_list.h"
#include "k_game/room_callback_alarm.h"
#include "k_game/room_callback_step.h"
#include "k_game/room_callback_draw.h"

struct k_room;

/* region [room_registry] */

int k__room_registry_init(void);

void k__room_registry_cleanup(void);

int k__room_registry_add(struct k_room *room, const char *room_name);

void k__room_registry_del(struct k_room *room);

/* endregion */

/* region [room] */

struct k_room {

    struct k_asset_registry_node registry_node;

    struct k_room_callback_list enter_callbacks;

    struct k_room_callback_list leave_callbacks;

    struct k_room_step_callback_registry step_callbacks;

    struct k_room_alarm_callback_storage alarm_callbacks;

    struct k_room_draw_callback_storage draw_callbacks;

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

#endif
