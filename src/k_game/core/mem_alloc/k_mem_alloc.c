#include <stdlib.h>

#include "k_log.h"

#include "k_game/core/k_mem_alloc.h"

void *k_mem_alloc(size_t size) {

    void *p = malloc(size);
    if (NULL == p) {
        k_log_error("malloc( %zu ) failed", size);
    }

    return p;
}

void k_mem_free(void *p) {
    free(p);
}
