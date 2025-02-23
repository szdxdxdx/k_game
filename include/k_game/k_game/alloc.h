#ifndef K_GAME_ALLOC_H
#define K_GAME_ALLOC_H

#include <stdlib.h>

#include "k_log.h"

static inline void *k_malloc(size_t size) {
    void *p = malloc(size);

#if K_LOG_LEVEL <= K_LOG_LEVEL_ERROR
    if (NULL == p)
        k_log_error("malloc( %zu ) failed", size);
#endif

    return p;
}

static inline void k_free(void *ptr) {
    free(ptr);
}

#endif
