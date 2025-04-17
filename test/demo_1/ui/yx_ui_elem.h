#ifndef YX_UI_ELEM_H
#define YX_UI_ELEM_H

#include "k_list.h"

#include "./yx_ui_val.h"

#include "./yx_ui_elem_array.h"

struct yx_ui_context;
struct yx_ui_elem_v_tbl;

struct yx_ui_elem {

    struct yx_ui_context *ui;

    const char *elem_type_name;

    struct yx_ui_elem *parent;
    struct yx_ui_elem_array children;

    float x;
    float y;

    struct yx_ui_val w;
    struct yx_ui_val h;

    struct yx_ui_elem_v_tbl *v_tbl;
};

struct yx_ui_elem_v_tbl {

    void (*fn_measure)(struct yx_ui_elem *elem);

    void (*fn_layout)(struct yx_ui_elem *elem);
};

struct yx_ui_elem *yx__ui_elem_construct(struct yx_ui_elem *elem, struct yx_ui_context *ui);

void yx__ui_elem_destruct(struct yx_ui_elem *elem);

#endif
