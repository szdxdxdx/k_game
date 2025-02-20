#include "SDL.h"

#include "k_log.h"

#include "k_game/room.h"
#include "../game/k_game_context.h"
#include "../game/k_game_SDL_event.h"
#include "./k_room_context.h"

static int enter_room(struct k_room *room) {

    k_game.current_ms = SDL_GetTicks();
    k_game.delta_ms = 0;

    /* 开启游戏循环，在 entry_event 回调中也能退出循环 */
    room->game_loop = 1;

    k__room_callback_list_exec_all(&room->enter_callbacks);

    return 0;
}

static void leave_room(struct k_room *room) {

    k_game.current_ms = SDL_GetTicks();
    k_game.delta_ms = 0;

    k__room_callback_list_exec_all(&room->leave_callbacks);
}

static inline int frame_delay(struct k_room *room) {

    Uint64 current_time = SDL_GetTicks64();
    Uint64 elapsed_time = current_time - k_game.current_ms;

#if 1
    /* 若已到新一帧的时间，则函数返回 0，否则返回非 0。
     * 不执行 SDL_Delay() 控制帧率。
     */
    if (elapsed_time < room->step_interval_ms)
        return 1;
#else
    if (elapsed_time < room->frame_interval) {

        /* 执行 SDL_Delay() 期间会暂停执行窗口消息循环，窗口无法响应用户的操作。
         * 若极端地将 fps 设置为很小的数（比如设为 1），用户能明显感觉到拖动窗口的响应卡顿。
         */
        SDL_Delay(room->frame_interval - elapsed_time);

        current_time = SDL_GetTicks();
        elapsed_time = current_time - room->ctx.current_time;
    }
#endif

    k_game.delta_ms   = (int)elapsed_time;
    k_game.current_ms = current_time;
    return 0;
}

static void room_step(struct k_room *room) {

    k__room_callback_list_exec_all(&room->step_begin_callbacks);

    k__room_exec_alarm_callbacks(room);

    k__room_callback_list_exec_all(&room->step_callbacks);

    // SDL_SetRenderDrawColor(room->renderer, 0, 0, 0, 255);
    // SDL_RenderClear(room->renderer);

    k__room_exec_draw_callbacks(room);

    SDL_RenderPresent(k_game.renderer);

    k__room_callback_list_exec_all(&room->step_end_callbacks);
}

static void game_loop(struct k_room *room) {

}

void k__room_run(struct k_room *room) {
    k_log_trace("Entering room { .name=\"%s\" }", k_room_get_name(room));

    k_game.current_room = room;
    enter_room(room);

    if (room->game_loop) {
        k_log_trace("Game loop started...");

        k_game.current_ms = SDL_GetTicks();
        while (room->game_loop) {

            do {
                k__game_process_SDL_events();
            } while (frame_delay(room));

            room_step(room);
        }

        k_log_trace("Game loop ended");
    }

    leave_room(room);
    k_game.current_room = NULL;

    k_log_trace("Left room { .name=\"%s\" }", k_room_get_name(room));
}
