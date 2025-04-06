#include "k_game/core/k_view.h"
#include "k_game/core/k_time.h"

#include "./k_room.h"
#include "./k_room_run.h"

#include "../k_SDL/_shared.h"
#include "../game/k_game_context.h"

void k__room_run(struct k_room *room) {
    k__game.current_room = room;

    room->game_loop = 1;

    k_view_set_position(0, 0);
    k_view_set_w((float)k__game.window_w);

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

        SDL_SetRenderTarget(k__game.renderer, k__game.canvas);

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

        SDL_SetRenderTarget(k__game.renderer, NULL);

        canvas_view.w = (int)k__game.view_w;
        canvas_view.h = (int)k__game.view_h;
        SDL_RenderCopyF(k__game.renderer, k__game.canvas, &canvas_view, NULL);

        SDL_RenderPresent(k__game.renderer);
    }

    if (NULL != room->fn_leave) {
        room->fn_leave();
    }

    k__game.current_room = NULL;
}
