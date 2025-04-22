#define K_LOG_TAG "llk:UI"
#include "k_log.h"

#include "k_mem_pool.h"

#include "./llk_ui_context.h"

void *llk__ui_mem_alloc(struct llk_ui_context *ui, size_t size) {

    void *p = k_mem_pool_alloc(&ui->mem_pool, size);
    if (NULL == p) {
        k_log_error("Failed to allocate memory");
    }

    return p;
}

void llk__ui_mem_free(void *p) {
    return k_mem_pool_free(p);
}
