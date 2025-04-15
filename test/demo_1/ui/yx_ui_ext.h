#ifndef YX_UI_EXT_H
#define YX_UI_EXT_H

#include "k_list.h"

#include "./yx_ui.h"

void *yx__ui_mem_alloc(struct yx_ui_context *ui, size_t size);

void yx__ui_mem_free(struct yx_ui_context *ui, void *p);

struct yx_ui_elem {

    struct k_list_node sibling_node;

    struct k_list child_list;

    struct yx_ui_context *ui;

    void (*fn_draw)(struct yx_ui_elem *elem);

    int (*fn_set_attr)(struct yx_ui_elem *elem, const char *key, const char *val);
};

int yx__ui_elem_construct(struct yx_ui_elem *elem);

void yx__ui_elem_destruct(struct yx_ui_elem *elem);

#endif
