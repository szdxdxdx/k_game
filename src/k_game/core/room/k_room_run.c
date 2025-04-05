#include "./_internal.h"

#include "../k_SDL/_public.h"
#include "../game/_public.h"

void k__room_run(struct k_room *room) {
    K_CURRENT_ROOM = room;

    room->game_loop = 1;

    if (NULL != room->fn_enter)
        room->fn_enter();

    k__game.step_timestamp = k_get_timestamp();
    k__game.step_delta_ms  = 0;
    k__game.step_delta     = 0.0f;
    while (room->game_loop) {

        k__handle_SDL_event_with_frame_delay(room->step_interval_ms);

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

        SDL_Rect view = {
            .x = room->view_x,
            .y = room->view_y,
            .w = room->view_w,
            .h = room->view_h,
        };
        SDL_RenderCopyF(k__SDL.renderer, k__SDL.canvas, &view, NULL);
        SDL_RenderPresent(k__SDL.renderer);
    }

    if (NULL != room->fn_leave)
        room->fn_leave();

    K_CURRENT_ROOM = NULL;
}
