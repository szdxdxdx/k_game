#ifndef K_GAME_ALLOC_H
#define K_GAME_ALLOC_H

#include <stddef.h>

void *k_malloc(size_t size);

void k_free(void *ptr);

#endif
