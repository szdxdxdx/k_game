#include "./_internal.h"

#include "../k_SDL/_public.h"
#include "../game/_public.h"

void k__room_run(struct k_room *room) {
    k__game.current_room = room;

    room->game_loop = 1;

    if (NULL != room->fn_enter)
        room->fn_enter();

    k__game.step_timestamp = k_get_timestamp();
    while (room->game_loop) {

        k__handle_SDL_event_with_frame_delay(room->step_interval_ms);

        k__step_callback_manager_flush(&room->step_begin_callback_manager);
        k__step_callback_manager_exec(&room->step_begin_callback_manager);

        k__alarm_callback_manager_flush(&room->alarm_callback_manager);
        k__alarm_callback_manager_exec(&room->alarm_callback_manager);

        k__step_callback_manager_flush(&room->step_callback_manager);
        k__step_callback_manager_exec(&room->step_callback_manager);

        k__draw_callback_manager_flush(&room->draw_callback_manager);
        k__draw_callback_manager_exec(&room->draw_callback_manager);

        SDL_RenderPresent(k__window.renderer);

        k__step_callback_manager_flush(&room->step_end_callback_manager);
        k__step_callback_manager_exec(&room->step_end_callback_manager);
    }

    if (NULL != room->fn_leave)
        room->fn_leave();

    k__game.current_room = NULL;
}
