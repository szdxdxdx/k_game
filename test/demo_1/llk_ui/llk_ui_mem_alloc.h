#ifndef LLK_UI_MEM_ALLOC_H
#define LLK_UI_MEM_ALLOC_H

#include "./llk_ui_fwd.h"

void *llk__ui_mem_alloc(struct llk_ui_context *ui, size_t size);

void llk__ui_mem_free(void *p);

char *llk__ui_strdup(struct llk_ui_context *ui, const char *s, size_t len);

#endif
