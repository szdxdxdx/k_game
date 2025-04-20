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

    struct llk_ui_float w;
    struct llk_ui_float h;

    struct llk_ui_float left;
    struct llk_ui_float right;
    struct llk_ui_float top;
    struct llk_ui_float bottom;

    float x;
    float y;

    uint32_t background_color;
    uint32_t border_color;

    const struct llk_ui_elem_type_info *type;
};

struct llk_ui_elem *llk__ui_construct_elem(struct llk_ui_elem *elem, struct llk_ui_context *ui, const struct llk_ui_elem_type_info *type_info);

struct llk_ui_elem *llk__ui_destruct_elem(struct llk_ui_elem *elem);

struct llk_ui_elem *llk_ui_create_elem(struct llk_ui_context *ui, const char *type_name);

int llk_ui_elem_set_attr(struct llk_ui_elem *elem, const char *key, const char *val);

int llk_ui_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child);

void llk__ui_elem_measure(struct llk_ui_elem *elem);

void llk__ui_elem_layout(struct llk_ui_elem *elem);

void llk__ui_elem_draw(struct llk_ui_elem *elem);

struct llk_ui_elem_type_info {

    const char *type_name;

    size_t elem_size;

    struct llk_ui_elem *(*fn_construct)(void *elem_);

    void *(*fn_destruct)(struct llk_ui_elem *elem);

    void (*fn_draw)(struct llk_ui_elem *elem);
};

struct llk_ui_elem_type_config {

    const char *type_name;

    size_t elem_size;

    struct llk_ui_elem *(*fn_construct)(void *elem_);

    void *(*fn_destruct)(struct llk_ui_elem *elem);

    void (*fn_draw)(struct llk_ui_elem *elem);
};

int llk_ui_register_elem_type(struct llk_ui_context *ui, const struct llk_ui_elem_type_config *config);

#endif
