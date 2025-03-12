#include "SDL.h"

#include "k_game_room.h"

#include "../k_SDL/k_SDL.h"
#include "../game/k_game_context.h"

#include "./k_room.h"

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

void k__room_run(struct k_room *room) {
    k__game.current_room = room;

    room->game_loop = 1;

    if (NULL != room->fn_enter)
        room->fn_enter();

    while (room->game_loop) {

        k__callback_flush_step(&room->step_begin_callback_manager);
        k__callback_flush_alarm(&room->alarm_callback_manager);
        k__callback_flush_step(&room->step_callback_manager);
        k__callback_flush_draw(&room->draw_callback_manager);
        k__callback_flush_step(&room->step_end_callback_manager);

        k__refresh_keyboard();

        do k__poll_SDL_events(); while (frame_delay(room));

        k__callback_exec_step(&room->step_begin_callback_manager);
        k__callback_exec_alarm(&room->alarm_callback_manager);
        k__callback_exec_step(&room->step_callback_manager);
        k__callback_exec_draw(&room->draw_callback_manager);
        SDL_RenderPresent(k__window.renderer);
        k__callback_exec_step(&room->step_end_callback_manager);
    }

    if (NULL != room->fn_leave)
        room->fn_leave();

    k__game.current_room = NULL;
}
