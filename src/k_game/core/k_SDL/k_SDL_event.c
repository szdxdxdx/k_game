#include "./_internal.h"

#include "../game/_public.h"
#include "../room/_public.h"
#include "SDL_timer.h"

void k__poll_SDL_events(void) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT:
                k__game.quit_game = 1;
                K_CURRENT_ROOM->game_loop = 0;
                break;

            case SDL_MOUSEMOTION:
                k__handle_SDL_event_mouse_motion(&event.motion);
                break;
            case SDL_MOUSEBUTTONDOWN:
                k__handle_SDL_event_mouse_button_down(&event.button);
                break;
            case SDL_MOUSEBUTTONUP:
                k__handle_SDL_event_mouse_button_up(&event.button);
                break;

            case SDL_MOUSEWHEEL:
                // k__handle_SDL_event_mouse_wheel(&event.wheel);
                break;

            case SDL_KEYDOWN:
                /* FIXME: 键位冲突
                 * 同时按下多个按键时，SDL 可能无法识别所有按键
                 */
                k__handle_SDL_event_key_down(&event.key);
                break;
            case SDL_KEYUP:
                k__handle_SDL_event_key_up(&event.key);
                break;

            default:
                break;
        }
    }
}

void k__handle_SDL_event_with_frame_delay(uint64_t room_step_interval_ms) {

    k__refresh_keys_state();
    k__refresh_mouse_state();

    uint64_t current_time;
    uint64_t elapsed_time;

    do {
        k__poll_SDL_events();

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
