#ifndef LLK_UI_CONTEXT_H
#define LLK_UI_CONTEXT_H

#include "k_mem_pool.h"
#include "k_str_map.h"

#include "./llk_ui_fwd.h"

struct llk_ui_context {

    /* 视口宽高，即 UI 画布的宽高 */
    float vw;
    float vh;

    /* 鼠标在视口的坐标 */
    float mouse_x;
    float mouse_y;

    /* 鼠标是否按下或按住 */
    unsigned int mouse_button_pressed;
    unsigned int mouse_button_down;

    /* 根元素节点，elem type 固定是 `root` */
    struct llk_ui_elem *root;

    /* 内存池 */
    struct k_mem_pool mem_pool;

    /* 记录已注册的元素类型 */
    struct k_str_map elem_type_map;

    /* 用于通过 id 查找元素 */
    struct k_str_intrusive_map elem_id_map;

    /* 记录已注册的函数 */
    struct k_str_map callback_fn_map;

    /* 布局脏标记。若为非 0，则重新计算所有元素的尺寸和位置 */
    int layout_dirty;
};

struct llk_ui_context *llk_ui_create_context(void);

void llk_ui_destroy_context(struct llk_ui_context *ui);

struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui);

/* 标记 UI 结构或布局发生了变更，需要重新布局 */
void llk__ui_mark_layout_dirty(struct llk_ui_context *ui);

/* 布局元素，计算所有元素的尺寸和位置 */
void llk__ui_layout(struct llk_ui_context *ui);

/* 布局元素，计算所有元素的尺寸和位置 */
void llk__ui_update(struct llk_ui_context *ui);

void llk_ui_draw(struct llk_ui_context *ui);

void llk_ui_update(struct llk_ui_context *ui);

typedef void (*llk_ui_callback_fn)(void);

int llk_ui_register_callback(struct llk_ui_context *ui, const char *key, llk_ui_callback_fn fn_callback);

llk_ui_callback_fn llk__ui_get_callback(struct llk_ui_context *ui, const char *key);

#endif
