#include "SDL.h"

#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

static int enter_room(struct k_room *room) {

    room->current_time = SDL_GetTicks();
    room->delta_ms = 0;

    /* 开启游戏循环，在 entry_event 回调中也能退出循环 */
    room->game_loop = 1;

    if (NULL != room->fn_enter) {

        int result = room->fn_enter(room);
        if (0 != result) {
            k_log_error("Failed to enter room { .name=\"%s\" }. "
                        "Room fn_entry() callback returned %d", room->name, result);
            return -1;
        }
    }

    return 0;
}

static void leave_room(struct k_room *room) {

    room->current_time = SDL_GetTicks();
    room->delta_ms = 0;

    if (NULL != room->fn_leave)
        room->fn_leave(room);
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

    room->delta_ms = (int)elapsed_time;
    room->current_time = current_time;
    return 0;
}

static void process_SDL_events(struct k_room *room) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT:
                k__game.quit_game = 1;
                room->game_loop = 0;
                break;

            default:
                break;
        }
    }
}

static void room_step(struct k_room *room) {

    if (NULL != room->fn_step)
        room->fn_step(room);

    // SDL_SetRenderDrawColor(room->renderer, 0, 0, 0, 255);
    // SDL_RenderClear(room->renderer);

    if (NULL != room->fn_draw)
        room->fn_draw(room);

    SDL_RenderPresent(k__game.renderer);
}

void k__run_room(struct k_room *room) {
    k_log_trace("Entering room { .name=\"%s\" }", room->name);

    if (0 != enter_room(room))
        return;

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

    k_log_trace("Left room { .name=\"%s\" }", room->name);
}
