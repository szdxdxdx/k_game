#ifndef LLK_UI_CONTEXT_H
#define LLK_UI_CONTEXT_H

#include "./llk_ui_fwd.h"

#include "k_mem_pool.h"

struct llk_ui_context {

    struct llk_ui_elem *root;

    float vw;
    float vh;

    struct k_mem_pool mem_pool;
};

struct llk_ui_context *llk_ui_create_context(void);

void llk_ui_destroy_context(struct llk_ui_context *ui);

void *llk__ui_mem_alloc(struct llk_ui_context *ui, size_t size);

void llk__ui_mem_free(void *p);

struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui);

void llk_ui_draw(struct llk_ui_context *ui);

#endif
