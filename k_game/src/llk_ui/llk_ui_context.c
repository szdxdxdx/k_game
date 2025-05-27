#include <stdlib.h>
#include <assert.h>

#define K_LOG_TAG "llk:UI"
#include "k_log.h"

#include "k_game/core/k_mouse.h"
#include "k_game/core/k_canvas.h"

#include "./llk_ui_ext.h"

/* region [create] */

/* 所有根的父元素，表示整个视口。只有 `x` 和 `y` 是有用的 */
static struct llk_ui_elem llk__ui_window = {
    .x = 0.0f,
    .y = 0.0f,
};

/* llk UI 的内建元素 */
extern struct llk_ui_elem_type llk__ui_elem_root;
extern struct llk_ui_elem_type llk__ui_elem_box;
extern struct llk_ui_elem_type llk__ui_elem_slider;
extern struct llk_ui_elem_type llk__ui_elem_image;

static int llk__ui_registry_builtin_elem_types(struct llk_ui_context *ui) {

    if (0 != llk__ui_register_elem_type(ui, &llk__ui_elem_root))
        return -1;
    if (0 != llk__ui_register_elem_type(ui, &llk__ui_elem_box))
        return -1;
    if (0 != llk__ui_register_elem_type(ui, &llk__ui_elem_image))
        return -1;
    if (0 != llk__ui_register_elem_type(ui, &llk__ui_elem_slider))
        return -1;

    return 0;
}

struct llk_ui_context *llk_ui_create_context(void) {

    struct llk_ui_context *ui = NULL;
    struct k_mem_pool *mem_pool = NULL;
    struct k_str_map *elem_type_map = NULL;
    struct k_str_intrusive_map *elem_id_map = NULL;
    struct k_str_map *callback_fn_map = NULL;

    ui = malloc(sizeof(struct llk_ui_context));
    if (NULL == ui)
        goto err;

    struct k_mem_pool_config config;
    config.fn_malloc        = malloc;
    config.fn_free          = free;
    config.alloc_size_align = 16;
    config.block_size_max   = sizeof(struct llk_ui_elem) * 2;
    config.alloc_chunk_size = 1024;
    mem_pool = k_mem_pool_construct(&ui->mem_pool, &config);
    if (NULL == mem_pool)
        goto err;

    elem_type_map = k_str_map_construct(&ui->elem_type_map, NULL);
    if (NULL == elem_type_map)
        goto err;

    size_t buckets_num = 16;
    struct k_hash_list *buckets = k_mem_pool_alloc(&ui->mem_pool, sizeof(struct k_hash_list) * buckets_num);
    if (NULL == buckets) {
        goto err;
    } else {
        k_str_intrusive_map_init(&ui->elem_id_map, buckets, buckets_num);
    }

    callback_fn_map = k_str_map_construct(&ui->callback_fn_map, NULL);
    if (NULL == callback_fn_map)
        goto err;

    if (0 != llk__ui_registry_builtin_elem_types(ui))
        goto err;

    float vw = k_canvas_ui_get_vw();
    float vh = k_canvas_ui_get_vh();

    struct llk_ui_elem *root = llk_ui_elem_create(ui, "root");
    if (NULL == root)
        goto err;

    root->w.unit = LLK_UI_UNIT_PX;
    root->w.specified_val = vw;
    root->w.computed_val  = vw;
    root->h.unit = LLK_UI_UNIT_PX;
    root->h.specified_val = vh;
    root->h.computed_val  = vh;
    root->parent = &llk__ui_window;

    ui->vw = vw;
    ui->vh = vh;
    ui->mouse_x = 0.0f;
    ui->mouse_y = 0.0f;
    ui->root = root;
    ui->layout_dirty = 1;

    return ui;

err:
    if (NULL != callback_fn_map)
        k_str_map_destruct(callback_fn_map);

    if (NULL != elem_type_map)
        k_str_map_destruct(elem_type_map);

    if (NULL != mem_pool)
        k_mem_pool_destruct(mem_pool);

    if (NULL != ui)
        free(ui);

    k_log_error("Failed to create UI context");
    return NULL;
}

void llk_ui_destroy_context(struct llk_ui_context *ui) {

    if (NULL == ui)
        return;

    /* TODO 销毁所有 UI 元素（包括哪些已创建出来但是没有添加到树上的元素） */
    ui->root->parent = NULL;
    llk__ui_elem_destroy(ui->root);

    k_str_map_destruct(&ui->elem_type_map);

    k_mem_pool_destruct(&ui->mem_pool);
    free(ui);
}

/* endregion */

/* region [get_elem] */

struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui) {
    return ui->root;
}

struct llk_ui_elem *llk_ui_get_elem_by_id(struct llk_ui_context *ui, const char *id) {

    if (NULL == ui || NULL == id)
        return NULL;

    struct k_str_intrusive_map_node *node = k_str_intrusive_map_get(&ui->elem_id_map, id);
    if (NULL == node)
        return NULL;

    struct llk_ui_elem *elem = container_of(node, struct llk_ui_elem, id_map_node);
    return elem;
}

/* endregion */

/* region [update] */

static void llk__ui_get_input(struct llk_ui_context *ui) {

    ui->mouse_x = k_mouse_window_x();
    ui->mouse_y = k_mouse_window_y();

    if (k_mouse_button_down(K_BUTTON_LEFT)) {
        ui->mouse_button_down = 1;
    } else {
        ui->mouse_button_down = 0;
    }

    if (k_mouse_button_down_or_held(K_BUTTON_LEFT)) {
        ui->mouse_button_pressed = 1;
    } else {
        ui->mouse_button_pressed = 0;
    }
}

void llk__ui_mark_layout_dirty(struct llk_ui_context *ui) {
    ui->layout_dirty = 1;
}

void llk__ui_layout_if_dirty(struct llk_ui_context *ui) {

    if (ui->layout_dirty) {
        llk__ui_elem_measure(ui->root);
        llk__ui_elem_layout(ui->root);
        ui->layout_dirty = 0;
        llk__ui_elem_hit_test(ui->root);
    }
}

void llk_ui_update(struct llk_ui_context *ui) {

    llk__ui_get_input(ui);

    if (ui->layout_dirty) {
        llk__ui_elem_measure(ui->root);
        llk__ui_elem_layout(ui->root);

        ui->layout_dirty = 0;
    }

    llk__ui_elem_hit_test(ui->root);

    llk__ui_elem_dispatch_event(ui->root);
}

/* endregion */

/* region [draw] */

void llk_ui_draw(struct llk_ui_context *ui) {

    if (ui->layout_dirty) {
        llk__ui_elem_measure(ui->root);
        llk__ui_elem_layout(ui->root);
        ui->layout_dirty = 0;
    }

    struct llk_ui_elem *child;
    struct k_list *child_list = &ui->root->child_list;
    struct k_list_node *iter;

    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);
        llk__ui_elem_measure(child);
    }

    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);
        llk__ui_elem_layout(child);
    }

    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);
        llk__ui_elem_draw(child);
    }
}

/* endregion */

/* region [callback] */

int llk_ui_register_callback(struct llk_ui_context *ui, const char *key, void *fn_callback) {

    if (NULL != k_str_map_get(&ui->callback_fn_map, key)) {
        k_log_error("Callback function `%s` already registered", key);
        return -1;
    }

    void *val = k_str_map_add(&ui->callback_fn_map, key, sizeof(void *));
    if (NULL == val) {
        k_log_error("Failed to register callback function `%s`", key);
        return -1;
    }

    *(void **)val = fn_callback;

    return 0;
}

void *llk__ui_get_callback(struct llk_ui_context *ui, const char *key) {
    assert(NULL != key && '\0' != key[0]);

    void *val = k_str_map_get(&ui->callback_fn_map, key);
    if (NULL == val) {
        k_log_warn("callback function `%s` not registered", key);
        return NULL;
    }

    return *(void **)val;
}

/* endregion */
