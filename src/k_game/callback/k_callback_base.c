#include "./k_callback_base.h"

void k__callback_set_deleted(struct k_callback *callback) {

    /* TODO assert( ! callback->deleted) */

    callback->deleted = 1;
}
