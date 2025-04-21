#ifndef LLK_UI_ELEM_TYPE_H
#define LLK_UI_ELEM_TYPE_H

#include <stddef.h>

#include "./llk_ui_fwd.h"

struct llk_ui_elem_type_config {

    const char *type_name;

    size_t data_size;

    int (*fn_init)(struct llk_ui_elem *elem);

    void (*fn_fini)(struct llk_ui_elem *elem);

    int (*fn_set_attr)(struct llk_ui_elem *elem, const char *key, const char *val);

    void (*fn_draw)(struct llk_ui_elem *elem);
};

int llk_ui_register_elem_type(struct llk_ui_context *ui, const struct llk_ui_elem_type_config *config);

struct llk_ui_elem_type {

    const char *type_name;

    size_t data_size;

    int (*fn_init)(struct llk_ui_elem *elem);

    void (*fn_fini)(struct llk_ui_elem *elem);

    int (*fn_set_attr)(struct llk_ui_elem *elem, const char *key, const char *val);

    void (*fn_draw)(struct llk_ui_elem *elem);
};

#endif
