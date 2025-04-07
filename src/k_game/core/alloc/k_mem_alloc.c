#include <stdlib.h>

#include "k_log.h"

#include "k_game/core/k_alloc.h"

void *k_malloc(size_t size) {

    void *p = malloc(size);
    if (NULL == p)
        k_log_error("malloc( %zu ) failed", size);

    return p;
}

void k_free(void *p) {
    free(p);
}
