#include "SDL.h"

#include "k_log.h"

#include "k_game/room.h"
#include "../SDL/k_SDL_event.h"
#include "../SDL/k_SDL_window.h"
#include "../SDL/k_SDL_keyboard.h"
#include "../game/k_game_context.h"
#include "./k_room_context.h"
#include "./k_room_run.h"

static int enter_room(struct k_room *room) {

    /* 开启游戏循环，在 entry_event 回调中也能退出循环 */
    room->game_loop = 1;

    k__room_callback_list_exec_all(&room->enter_callbacks);

    return 0;
}

static void leave_room(struct k_room *room) {
    k__room_callback_list_exec_all(&room->leave_callbacks);
}

static inline int frame_delay(struct k_room *room) {

    uint64_t current_time = SDL_GetTicks64();
    uint64_t elapsed_time = current_time - k__game.step_timestamp;

    /* 若已到新一帧的时间，则函数返回 0，否则返回非 0
     *
     * 不使用 `SDL_Delay()` 来控制帧率，因为 delay 期间不执行窗口消息循环，窗口无法响应操作。
     * 例如，若极端地将帧率设为 1 秒 1 帧时，用户能明显感觉到拖动窗口的响应卡顿。
     */
    if (elapsed_time < room->step_interval_ms)
        return 1;

    k__game.step_timestamp = current_time;
    k__game.step_delta_ms = (int)elapsed_time;
    k__game.step_delta = (float)elapsed_time / 1000.0f;
    return 0;
}

static void game_loop(struct k_room *room) {
    k_log_trace("Game loop started...");

    while (room->game_loop) {

        k__refresh_keyboard();

        do k__poll_SDL_events(); while (frame_delay(room));

        k__room_callback_list_exec_all(&room->step_begin_callbacks);

        k__room_exec_alarm_callbacks(room);

        k__room_callback_list_exec_all(&room->step_callbacks);

        SDL_SetRenderDrawColor(k__window.renderer, 0, 0, 0, 255);
        SDL_RenderClear(k__window.renderer);

        k__room_exec_draw_callbacks(room);

        SDL_RenderPresent(k__window.renderer);

        k__room_callback_list_exec_all(&room->step_end_callbacks);
    }

    k_log_trace("Game loop ended");
}

void k__room_run(struct k_room *room) {
    k_log_trace("Entering room { .name=\"%s\" }", k_room_get_name(room));

    k__game.current_room = room;
    enter_room(room);

    if (room->game_loop)
        game_loop(room);

    leave_room(room);
    k__game.current_room = NULL;

    k_log_trace("Left room { .name=\"%s\" }", k_room_get_name(room));
}
