#include <assert.h>

#include "./_internal.h"

void k__del_callback(struct k_callback_base *callback) {

    switch (callback->event) {
        case K_ALARM_CALLBACK:
            k__del_alarm_callback(callback);
            break;
        case K_STEP_CALLBACK:
            k__del_step_callback(callback);
            break;
        case K_DRAW_CALLBACK:
            k__del_draw_callback(callback);
            break;
        default:
            assert(0);
    }
}
