#include <assert.h>

#include "./_internal.h"

void k__callback_del(struct k_callback_base *callback) {

    switch (callback->event) {
        case K_ALARM_CALLBACK:
            k__callback_del_alarm(callback);
            break;
        case K_STEP_CALLBACK:
            k__callback_del_step(callback);
            break;
        case K_DRAW_CALLBACK:
            k__callback_del_draw(callback);
            break;
        default:
            assert(0);
    }
}
