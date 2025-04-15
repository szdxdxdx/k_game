#ifndef YX_UI_ELEM_H
#define YX_UI_ELEM_H

#include "k_list.h"

struct yx_ui_context;
struct yx_ui_elem_v_tbl;

struct yx_ui_elem {

    struct k_list_node sibling_link;

    struct k_list child_list;

    struct yx_ui_context *ui;

    struct yx_ui_elem_v_tbl *v_tbl;
};

struct yx_ui_elem_v_tbl {

    void (*fn_destruct)(struct yx_ui_elem *elem);

    void (*fn_draw)(struct yx_ui_elem *elem);

    int (*fn_set_attr)(struct yx_ui_elem *elem, const char *key, const char *val);
};

int yx__ui_elem_construct(struct yx_ui_elem *elem, struct yx_ui_context *ui);

void yx__ui_elem_destruct(struct yx_ui_elem *elem);

#endif
