#include <stddef.h>

#include "./k_room_callback.h"

void k_room_del_callback(struct k_room_callback *callback) {

    if (NULL != callback)
        callback->fn_del_self(callback);
}
