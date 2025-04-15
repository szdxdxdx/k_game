#ifndef YX_UI_H
#define YX_UI_H

struct yx_ui;

struct yx_ui_elem;

struct yx_ui *yx_ui_create_context(void);

struct yx_ui_elem *yx_ui_create_elem(const char *elem_type);

#endif
