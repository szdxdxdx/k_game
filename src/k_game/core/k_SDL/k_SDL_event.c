#include "./_internal.h"

#include "../game/k_game_context.h"
#include "../room/k_room.h"
#include "SDL_timer.h"

static void k__SDL_poll_events(void) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT:
                k__game.quit_game = 1;
                k__game.current_room->game_loop = 0;
                break;

            case SDL_MOUSEMOTION:
                k__SDL_handle_event_mouse_motion(&event.motion);
                break;
            case SDL_MOUSEBUTTONDOWN:
                k__SDL_handle_event_mouse_button_down(&event.button);
                break;
            case SDL_MOUSEBUTTONUP:
                k__SDL_handle_event_mouse_button_up(&event.button);
                break;

            case SDL_KEYDOWN:
                /* FIXME: 键位冲突
                 * 同时按下多个按键时，SDL 可能无法识别所有按键
                 */
                k__SDL_handle_event_key_down(&event.key);
                break;
            case SDL_KEYUP:
                k__SDL_handle_event_key_up(&event.key);
                break;

            default:
                break;
        }
    }
}

void k__SDL_handle_event_with_frame_delay(uint64_t room_step_interval_ms) {

    k__SDL_refresh_keyboard_state();
    k__SDL_refresh_mouse_state();

    uint64_t current_time;
    uint64_t elapsed_time;

    do {
        k__SDL_poll_events();

        current_time = SDL_GetTicks64();
        elapsed_time = current_time - k__game.step_timestamp;

        /* 不使用 `SDL_Delay()` 来控制帧率，是因为 delay 期间不执行代码，窗口无法响应操作。
         * 例如，若极端地将帧率设为 1 秒 1 帧时，用户能明显感觉到拖动窗口的响应卡顿。
         */
    } while (elapsed_time < room_step_interval_ms);

    k__game.step_timestamp = current_time;
    k__game.step_delta_ms  = (int)elapsed_time;
    k__game.step_delta     = (float)elapsed_time / 1000.0f;
}
