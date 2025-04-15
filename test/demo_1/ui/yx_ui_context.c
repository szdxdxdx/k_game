#include <stdlib.h>

#include "k_game/core/k_object.h"

#include "./yx_ui.h"

#include "./yx_ui_context.h"
#include "./yx_ui_elem.h"
#include "./yx_ui_elem_root.h"

struct yx_ui_context {

    struct k_object *object;

    struct yx_ui_elem *root;

    struct k_mem_pool mem_pool;
};

/* region [mem_alloc] */

void *yx__ui_mem_alloc(struct yx_ui_context *ui, size_t size) {
    return k_mem_pool_alloc(&ui->mem_pool, size);
}

void yx__ui_mem_free(struct yx_ui_context *ui, void *p) {
    k_mem_pool_free(&ui->mem_pool, p);
}

/* endregion */

/* region [context_create] */

struct yx_ui_context *yx_ui_create_context(void) {

    struct k_object *object = NULL;
    struct k_mem_pool *pool = NULL;
    struct yx_ui_elem *root = NULL;

    object = k_object_create(sizeof(struct yx_ui_context));
    if (NULL == object)
        goto err;

    struct yx_ui_context *ui = k_object_get_data(object);

    ui->object = object;

    {
        struct k_mem_pool_config pool_conf;
        pool_conf.fn_malloc        = malloc;
        pool_conf.fn_free          = free;
        pool_conf.alloc_size_align = 8;
        pool_conf.block_size_max   = 512;
        pool_conf.alloc_chunk_size = 4096;
        pool = k_mem_pool_construct(&ui->mem_pool, &pool_conf);
        if (NULL == pool)
            goto err;
    }

    {
        root = yx_ui_elem_root_create(ui);
        if (NULL == root)
            goto err;

        ui->root = root;
    }

    return ui;

err:
    if (NULL != object)
        k_object_destroy(object);
    if (NULL == pool)
        k_mem_pool_destruct(&ui->mem_pool);

    return NULL;
}

void yx_ui_destroy_context(struct yx_ui_context *ui) {
    /* TODO */
}

/* endregion */

/* region [root] */

struct yx_ui_elem *yx_ui_get_root(struct yx_ui_context *ui) {
    return ui->root;
}

/* endregion */

/* region [draw] */

void yx_ui_draw(struct yx_ui_context *ui) {
    yx__ui_elem_draw(ui->root);
}

/* endregion */
