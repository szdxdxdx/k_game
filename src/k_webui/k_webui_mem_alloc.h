#ifndef K__WEBUI_MEM_ALLOC_H
#define K__WEBUI_MEM_ALLOC_H

#include <stddef.h>

void *k__webui_mem_alloc(size_t size);

void k__webui_mem_free(void *p);

#endif
