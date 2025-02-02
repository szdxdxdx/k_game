#ifndef K_MEM_ALLOC_H
#define K_MEM_ALLOC_H

#include <stdlib.h>

static inline void *k_malloc(size_t size) {
    void *p = malloc(size);

    if (NULL == p)
        k_log_error("malloc( %zu ) failed", size);

    return p;
}

static inline void k_free(void *ptr) {
    free(ptr);
}

#endif
