#ifndef K_ALLOC_H
#define K_ALLOC_H

#include <stddef.h>

void *k_malloc(size_t size);

void k_free(void *p);

#endif
