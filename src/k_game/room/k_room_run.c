#include "SDL.h"

#include "k_log.h"

#include "k_game/room.h"
#include "../game/k_game_context.h"
#include "../room/k_room_context.h"
#include "./k_room_run.h"

static int enter_room(struct k_room *room) {

    room->current_time = SDL_GetTicks();
    room->delta_ms = 0;

    /* 开启游戏循环，在 entry_event 回调中也能退出循环 */
    room->game_loop = 1;

    /* 执行 enter callbacks */

    return 0;
}

static void leave_room(struct k_room *room) {

    room->current_time = SDL_GetTicks();
    room->delta_ms = 0;

    /* 执行 leave callbacks */
}

static inline int frame_delay(struct k_room *room) {

    /* TODO: 使用 uint64_t 记录时间，保留 int 记录帧间隔时间 */

    Uint64 current_time = SDL_GetTicks64();
    Uint64 elapsed_time = current_time - room->current_time;

#if 1
    /* 若已到新一帧的时间，则函数返回 0，否则返回非 0。
     * 不执行 SDL_Delay() 控制帧率。
     */
    if (elapsed_time < room->frame_interval)
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

    k_game.delta_time = (float)(elapsed_time) / 1000.0f;
    room->delta_ms = (int)elapsed_time;
    room->current_time = current_time;
    return 0;
}

static void process_SDL_events(struct k_room *room) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT:
                k_game.quit_game = 1;
                room->game_loop = 0;
                break;

            default:
                break;
        }
    }
}

static void room_step(struct k_room *room) {

    /* 执行 step begin callbacks */

    /* 执行 alarm callbacks */

    /* 执行 step callbacks */

    // SDL_SetRenderDrawColor(room->renderer, 0, 0, 0, 255);
    // SDL_RenderClear(room->renderer);

    /* 执行 draw callbacks */

    SDL_RenderPresent(k_game.renderer);

    /* 执行 step end callbacks */
}

void k_run_room(struct k_room *room) {
    k_log_trace("Entering room { .name=\"%s\" }", k_room_get_name(room));

    enter_room(room);

    if (room->game_loop) {
        k_log_trace("Game loop started...");

        room->current_time = SDL_GetTicks();
        while (room->game_loop) {

            do {
                process_SDL_events(room);
            } while (frame_delay(room));

            room_step(room);
        }

        k_log_trace("Game loop Ended");
    }

    leave_room(room);

    k_log_trace("Left room { .name=\"%s\" }", k_room_get_name(room));
}
