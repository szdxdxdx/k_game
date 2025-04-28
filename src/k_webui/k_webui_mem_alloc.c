#include <stdlib.h>

#define K_LOG_TAG "k_webui/malloc"
#include "k_log.h"

#include "./k_webui_internal.h"

void *k__webui_mem_alloc(size_t size) {

    void *p = malloc(size);
    if (NULL == p) {
        k_log_error("malloc( %zu ) failed", size);
    }

    return p;
}

void k__webui_mem_free(void *p) {
    free(p);
}

char *k__webui_strdup(const char *str, size_t len) {

    char *copy = k__webui_mem_alloc(len + 1);
    if (NULL == copy)
        return NULL;

    strncpy(copy, str, len);
    copy[len] = '\0';

    return copy;
}
