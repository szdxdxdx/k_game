#include <assert.h>

#include "./k_callback.h"

void k__callback_flag_deleted(struct k_callback_base *callback) {

    switch (callback->event) {
        case K_ALARM_CALLBACK: k__callback_flag_deleted_alarm(callback); break;
        case K_STEP_CALLBACK:  k__callback_flag_deleted_step(callback);  break;
        case K_DRAW_CALLBACK:  k__callback_flag_deleted_draw(callback);  break;
        default:
            assert(0);
    }
}

void k__callback_flag_dead(struct k_callback_base *callback) {

    switch (callback->event) {
        case K_ALARM_CALLBACK: k__callback_flag_dead_alarm(callback); break;
        case K_STEP_CALLBACK:  k__callback_flag_dead_step(callback);  break;
        case K_DRAW_CALLBACK:  k__callback_flag_dead_draw(callback);  break;
        default:
            assert(0);
    }
}
