#include <assert.h>

#include "SDL_timer.h"

#include "./k_SDL_event.h"
#include "./k_time.h"
#include "./k_mouse.h"
#include "./k_keyboard.h"

#include "../game/k_game_context.h"
#include "../room/k_room.h"

static void k__SDL_poll_events(void) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT:
                k__game.quit_game = 1;
                k__room_current->game_loop = 0;
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        /* FIXME 无法改变窗口大小
                         *
                         * 2025-04-14
                         *
                         * 使用 d3d 作为渲染后端时，改变窗口大小会导致无法绘制图片。
                         * 切换窗口大小后，调用 `SDL_SetRenderTarget()` 将渲染器绑定到画布时。
                         * SDL 报错：`DEBUG: CreateTexture(D3DPOOL_DEFAULT): INVALIDCALL`。
                         * 之后无法绘制图片，但是绘制几何图形（线段、矩形等）是可以的。
                         *
                         * 使用 opengl 作为渲染后端时，则不会报错。可以在 SDL 初始化之前指定：
                         * `SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl")`
                         *
                         * 原本我希望，游戏窗口是一开始指定后就不能改的。现在想让它能改还有改不了了。
                         */
                        assert(0);
                        break;
                    default:
                        break;
                }
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

    k__keyboard_refresh_state();
    k__mouse_refresh_state();

#if 0

    k__SDL_poll_events();

    uint64_t current_time = SDL_GetTicks64();
    uint64_t elapsed_time = current_time - k__time.step_timestamp;

    if (elapsed_time < room_step_interval_ms) {

        SDL_Delay(room_step_interval_ms - elapsed_time);

        current_time = SDL_GetTicks64();
        elapsed_time = current_time - k__time.step_timestamp;
    }
#else
    uint64_t current_time;
    uint64_t elapsed_time;

    while (1) {
        current_time = SDL_GetTicks64();
        elapsed_time = current_time - k__time.step_timestamp;

        k__SDL_poll_events();

        if (elapsed_time >= room_step_interval_ms)
            break;

        SDL_Delay(1);
    }
#endif

    k__time.step_timestamp = current_time;
    k__time.step_delta_ms  = (int)elapsed_time;
    k__time.step_delta     = (float)elapsed_time / 1000.0f;
}
