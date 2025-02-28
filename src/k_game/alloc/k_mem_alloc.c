#include <stdlib.h>

#include "k_log.h"

#include "k_game/alloc.h"

void *k_malloc(size_t size) {

    void *p = malloc(size);
    if (NULL == p)
        k_log_error("malloc( %zu ) failed", size);

    return p;
}

void k_free(void *ptr) {
    free(ptr);
}
