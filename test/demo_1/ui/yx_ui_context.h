#ifndef YX_UI_CONTEXT_H
#define YX_UI_CONTEXT_H

#include "k_mem_pool.h"

struct yx_ui_elem;

struct yx_ui_context {

    struct k_object *object;

    struct yx_ui_elem *root;

    struct k_mem_pool mem_pool;
};

void *yx__ui_mem_alloc(struct yx_ui_context *ui, size_t size);

void yx__ui_mem_free(struct yx_ui_context *ui, void *p);

#endif
