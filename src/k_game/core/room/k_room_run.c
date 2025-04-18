#include "k_game/core/k_view.h"
#include "k_game/core/k_time.h"

#include "./k_room.h"
#include "./k_room_run.h"

#include "../k_SDL/k_SDL_event.h"
#include "../k_SDL/k_time.h"
#include "../k_SDL/k_window.h"
#include "../k_SDL/k_canvas.h"

struct k_room *k__current_room = NULL;

void k__room_run(struct k_room *room) {
    k__current_room = room;

    room->game_loop = 1;

    k_view_fit_rect(k__window.window_w, k__window.window_h);
    k_view_set_position(0, 0);

    if (NULL != room->fn_enter) {
        room->fn_enter();
    }

    k__time.step_timestamp = k_get_timestamp();
    k__time.step_delta_ms  = 0;
    k__time.step_delta     = 0.0f;
    while (room->game_loop) {

        k__SDL_handle_event_with_frame_delay(room->step_interval_ms);

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

        k__canvas_present();
    }

    if (NULL != room->fn_leave) {
        room->fn_leave();
    }

    k__current_room = NULL;
}
