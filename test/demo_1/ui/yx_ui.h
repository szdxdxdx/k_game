#ifndef YX_UI_H
#define YX_UI_H

struct yx_ui;

struct yx_ui_elem;

struct yx_ui *yx_ui_create_context(void);

void yx_ui_draw(struct yx_ui *ui);

struct yx_ui_elem *yx_ui_get_body(struct yx_ui *ui);

struct yx_ui_elem *yx_ui_create_elem(struct yx_ui *ui, const char *elem_type);

int yx_ui_set_attr(struct yx_ui_elem *elem, const char *key, const char *val);

int yx_ui_append_child(struct yx_ui_elem *elem, struct yx_ui_elem *child);

#endif
