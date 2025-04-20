#include "k_log.h"

#include "k_game/core/k_canvas.h"

#include "./llk_ui_elem.h"
#include "./llk_ui_context.h"

static struct llk_ui_elem_type_info llk__ui_elem_default_type_info = {
    .type_name    = "",
    .elem_size    = sizeof(struct llk_ui_elem),
    .fn_construct = NULL,
    .fn_destruct  = NULL,
    .fn_draw      = NULL,
};

struct llk_ui_elem *llk__ui_construct_elem(struct llk_ui_elem *elem, struct llk_ui_context *ui, const struct llk_ui_elem_type_info *type_info) {

    elem->ui = ui;

    elem->parent = NULL;
    k_list_init(&elem->child_list);
    k_list_node_loop(&elem->sibling_link);

    elem->w.unit = LLK_UI_UNIT_NO_VAL;
    elem->h.unit = LLK_UI_UNIT_NO_VAL;

    elem->left.unit   = LLK_UI_UNIT_NO_VAL;
    elem->right.unit  = LLK_UI_UNIT_NO_VAL;
    elem->top.unit    = LLK_UI_UNIT_NO_VAL;
    elem->bottom.unit = LLK_UI_UNIT_NO_VAL;

    elem->x = 0.0f;
    elem->y = 0.0f;

    elem->background_color = 0x00000000;
    elem->border_color     = 0x00000000;

    elem->type = type_info;

    return elem;
}

struct llk_ui_elem *llk_ui_create_elem(struct llk_ui_context *ui, const char *type_name) {

    struct llk_ui_elem_type_info *type_info = k_str_map_get(&ui->elem_type_map, type_name);
    if (NULL == type_info) {
        k_log_error("llk UI element type `%s` not registered", type_name);
        return NULL;
    }

    struct llk_ui_elem *elem = llk__ui_mem_alloc(ui, type_info->elem_size);
    if (NULL == elem)
        return NULL;

    llk__ui_construct_elem(elem, ui, type_info);

    return elem;
}

int llk_ui_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child) {

    if (NULL == parent || NULL == child)
        return -1;
    if (NULL != child->parent)
        return -1;
    if (child->ui != parent->ui)
        return -1;

    k_list_add_tail(&parent->child_list, &child->sibling_link);

    child->parent = parent;
    return 0;
}

void llk__ui_elem_layout(struct llk_ui_elem *elem) {

    struct llk_ui_elem *parent = elem->parent;

    if (elem->left.unit != LLK_UI_UNIT_NO_VAL) {
        elem->x = parent->x + elem->left.computed_val;
    }
    else if (elem->right.unit != LLK_UI_UNIT_NO_VAL) {
        elem->x = parent->x + parent->w.computed_val - elem->w.computed_val - elem->right.computed_val;
    }
    else {
        elem->x = parent->x;
    }

    if (elem->top.unit != LLK_UI_UNIT_NO_VAL) {
        elem->y = parent->y + elem->top.computed_val;
    }
    else if (elem->bottom.unit != LLK_UI_UNIT_NO_VAL) {
        elem->y = parent->y + parent->h.computed_val - elem->h.computed_val - elem->bottom.computed_val;
    }
    else {
        elem->y = parent->y;
    }

    struct llk_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);

        llk__ui_elem_layout(child);
    }
}

void llk__ui_elem_draw(struct llk_ui_elem *elem) {

    k_canvas_set_draw_color_rgba(elem->background_color);
    k_canvas_ui_fill_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);

    k_canvas_set_draw_color_rgba(elem->border_color);
    k_canvas_ui_draw_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);

    if (elem->type->fn_draw != NULL) {
        elem->type->fn_draw(elem);
    }

    struct llk_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);

        llk__ui_elem_draw(child);
    }
}
