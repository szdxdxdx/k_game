#ifndef YX_UI_ELEM_H
#define YX_UI_ELEM_H

#include "k_list.h"

#include "./yx_ui_ext.h"

struct yx_ui_elem *yx__ui_elem_create(struct yx_ui_context *ui, size_t data_size);

void yx__ui_elem_draw(struct yx_ui_elem *elem);

#endif
