#include <assert.h>
#include "SDL_timer.h"

#include "./k_SDL_event.h"
#include "./k_time.h"
#include "./k_mouse.h"
#include "./k_keyboard.h"

#include "../game/k_game_context.h"
#include "../room/k_room.h"
#include "k_window.h"

static void k__SDL_poll_events(void) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT:
                k__game.quit_game = 1;
                k__current_room->game_loop = 0;
                break;

            case SDL_WINDOWEVENT: {
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
            }

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

    uint64_t current_time;
    uint64_t elapsed_time;

    do {
        k__SDL_poll_events();

        current_time = SDL_GetTicks64();
        elapsed_time = current_time - k__time.step_timestamp;

        /* 不使用 `SDL_Delay()` 来控制帧率，是因为 delay 期间不执行代码，窗口无法响应操作。
         * 例如，若极端地将帧率设为 1 秒 1 帧时，用户能明显感觉到拖动窗口的响应卡顿。
         */
    } while (elapsed_time < room_step_interval_ms);

    k__time.step_timestamp = current_time;
    k__time.step_delta_ms  = (int)elapsed_time;
    k__time.step_delta     = (float)elapsed_time / 1000.0f;
}
