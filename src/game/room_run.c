#include "SDL.h"

#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

static int entry_room(struct k__room *room) {

    /* 开启游戏循环，在 entry_event 回调中也能退出循环 */
    room->game_loop = 1;

    if (NULL != room->fn_entry_event) {
        k_log_info("Invoking room fn_entry_event() callback...");

        int callback_result = room->fn_entry_event(&room->room);
        if (0 != callback_result) {
            k_log_error("Failed to enter room. room fn_entry_event() callback returned %d", callback_result);
            return -1;
        }

        k_log_info("Room fn_entry_event() callback completed");
    }

    room->room.current_time = SDL_GetTicks();
    room->room.delta_ms = 0;

    return 0;
}

static void leave_room(struct k__room *room) {

    if (NULL != room->fn_leave_event) {
        k_log_info("Invoking room fn_leave_event() callback...");

        room->fn_leave_event(&room->room);

        k_log_info("Room fn_leave_event() callback completed");
    }
}

static inline int frame_delay(struct k__room *room) {

    Uint32 current_time = SDL_GetTicks();
    Uint32 elapsed_time = current_time - room->room.current_time;

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

    room->room.delta_ms = (int)elapsed_time;
    room->room.current_time = current_time;
    return 0;
}

static void process_SDL_events(struct k__room *room) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT:
                k__game->quit_game = 1;
                room->game_loop = 0;
                break;

            default:
                break;
        }
    }
}

static void room_step(struct k__room *room) {

    if (NULL != room->fn_step_event)
        room->fn_step_event(&room->room);

    // SDL_SetRenderDrawColor(room->renderer, 0, 0, 0, 255);
    // SDL_RenderClear(room->renderer);

    if (NULL != room->fn_draw_event)
        room->fn_draw_event(&room->room);

    SDL_RenderPresent(k__game->renderer);
}

void k__run_room(struct k__room *room) {
    k_log_info("Entering room { .name=\"%s\", .id=%zu }", room->name, room->id);

    if (0 != entry_room(room))
        return;

    if (room->game_loop) {
        k_log_trace("Game loop started...");

        room->room.current_time = SDL_GetTicks();
        while (room->game_loop) {

            do {
                process_SDL_events(room);
            } while (frame_delay(room));

            room_step(room);
        }

        k_log_trace("Game loop Ended");
    }

    leave_room(room);

    k_log_info("Left room { .name=\"%s\", .id=%zu}", room->name, room->id);
}
