#ifndef YX_UI_H
#define YX_UI_H

#include "./yx_ui_internal.h"

struct yx_ui_context;

struct yx_ui_elem;

struct yx_ui_context *yx_ui_create_context(void);

struct yx_ui_elem *yx_ui_create_elem(struct yx_ui_context *ui, const char *elem_type);

int yx_ui_set_attr(struct yx_ui_elem *elem, const char *key, const char *val);

int yx_ui_append_child(struct yx_ui_elem *parent, struct yx_ui_elem *child);

int yx_ui_set_root(struct yx_ui_context *ui, struct yx_ui_elem *root);

void yx_ui_draw(struct yx_ui_context *ui);

#endif
