#ifndef K_MEM_ALLOC_H
#define K_MEM_ALLOC_H

#include <stddef.h>

void *k__mem_alloc(size_t size);

void k__mem_free(void *p);

#endif
