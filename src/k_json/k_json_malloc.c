#include <stdlib.h>

#include "./_internal.h"

void *k__json_mem_alloc(size_t size) {
    return malloc(size);
}

void k__json_mem_free(void *ptr) {
    free(ptr);
}

char *k__json_strdup(const char *str, size_t len) {

    char * str_copy = k__json_mem_alloc(len + 1);
    if (NULL == str_copy)
        return NULL;

    memcpy(str_copy, str, len);
    str_copy[len] = '\0';
    return str_copy;
}
