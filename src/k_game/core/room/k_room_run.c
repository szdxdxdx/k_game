#include "./_internal.h"

#include "../k_SDL/_shared.h"
#include "../view/_shared.h"
#include "../game/_shared.h"

void k__room_run(struct k_room *room) {
    K_CURRENT_ROOM = room;

    room->game_loop = 1;

    k_view_set_position(0, 0);
    k_view_set_w((float)k__SDL.window_w);

    if (NULL != room->fn_enter) {
        room->fn_enter();
    }

    SDL_Rect canvas_view;
    canvas_view.x = 0;
    canvas_view.y = 0;

    k__game.step_timestamp = k_get_timestamp();
    k__game.step_delta_ms  = 0;
    k__game.step_delta     = 0.0f;
    while (room->game_loop) {

        k__SDL_handle_event_with_frame_delay(room->step_interval_ms);

        SDL_SetRenderTarget(k__SDL.renderer, k__SDL.canvas);

        k__step_callback_manager_flush(&room->step_begin_callback_manager);
        k__step_callback_manager_exec(&room->step_begin_callback_manager);

        k__alarm_callback_manager_flush(&room->alarm_callback_manager);
        k__alarm_callback_manager_exec(&room->alarm_callback_manager);

        k__step_callback_manager_flush(&room->step_callback_manager);
        k__step_callback_manager_exec(&room->step_callback_manager);

        k__step_callback_manager_flush(&room->step_end_callback_manager);
        k__step_callback_manager_exec(&room->step_end_callback_manager);

        k__draw_callback_manager_flush(&room->draw_callback_manager);
        k__draw_callback_manager_exec(&room->draw_callback_manager);

        SDL_SetRenderTarget(k__SDL.renderer, NULL);

        canvas_view.w = (int)k__view.view_w;
        canvas_view.h = (int)k__view.view_h;
        SDL_RenderCopyF(k__SDL.renderer, k__SDL.canvas, &canvas_view, NULL);

        SDL_RenderPresent(k__SDL.renderer);
    }

    if (NULL != room->fn_leave) {
        room->fn_leave();
    }

    K_CURRENT_ROOM = NULL;
}
