#include <assert.h>

#include "./k_callback_alarm.h"
#include "./k_callback_step.h"
#include "./k_callback_draw.h"

void k__callback_del(struct k_callback_base *callback) {

    switch (callback->event) {
        case K_ALARM_CALLBACK:
            k__alarm_callback_manager_del_callback(callback);
            break;
        case K_STEP_CALLBACK:
            k__step_callback_manager_del_callback(callback);
            break;
        case K_DRAW_CALLBACK:
            k__draw_callback_manager_del_callback(callback);
            break;
        default:
            assert(0);
    }
}
