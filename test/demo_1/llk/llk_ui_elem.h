#ifndef LLK_UI_ELEM_H
#define LLK_UI_ELEM_H

#include <stdint.h>

#include "k_list.h"

#include "./llk_ui_fwd.h"

struct llk_ui_elem {

    struct llk_ui_context *ui;

    struct llk_ui_elem *parent;

    struct k_list child_list;

    struct k_list_node sibling_link;

    /* ------------------------------------------------------------------------ */

    const char *debug_info;

    /* ------------------------------------------------------------------------ */

    struct llk_ui_float w;
    struct llk_ui_float h;

    struct llk_ui_float left;
    struct llk_ui_float right;
    struct llk_ui_float top;
    struct llk_ui_float bottom;

    float x;
    float y;

    /* ------------------------------------------------------------------------ */

    uint32_t background_color;

    uint32_t border_color;
};

struct llk_ui_elem *llk__ui_construct_elem(struct llk_ui_elem *elem, struct llk_ui_context *ui);

struct llk_ui_elem *llk__ui_destruct_elem(struct llk_ui_elem *elem);

struct llk_ui_elem *llk_ui_create_elem(struct llk_ui_context *ui);

int llk_ui_elem_set_attr(struct llk_ui_elem *elem, const char *key, const char *val);

int llk_ui_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child);

void llk__ui_elem_measure(struct llk_ui_elem *elem);

void llk__ui_elem_layout(struct llk_ui_elem *elem);

void llk__ui_elem_draw(struct llk_ui_elem *elem);

#endif
