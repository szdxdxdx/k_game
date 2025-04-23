#include <string.h>

#define K_LOG_TAG "llk:UI"
#include "k_log.h"

#include "k_mem_pool.h"

#include "./llk_ui_ext.h"

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

char *llk__ui_strdup(struct llk_ui_context *ui, const char *s, size_t len) {

    char *s_new = llk__ui_mem_alloc(ui, len + 1);
    if (NULL == s_new)
        return NULL;

    memcpy(s_new, s, len);
    s_new[len] = '\0';

    return s_new;
}
