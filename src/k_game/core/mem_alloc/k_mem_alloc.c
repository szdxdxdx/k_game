#include <stdlib.h>

#define K_LOG_TAG "k_game:malloc"
#include "k_log.h"

#include "k_game/core/k_mem_alloc.h"

void *k__mem_alloc(size_t size) {

    void *p = malloc(size);
    if (NULL == p) {
        k_log_error("malloc( %zu ) failed", size);
    }

    return p;
}

void k__mem_free(void *p) {
    free(p);
}
