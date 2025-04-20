#include "k_game/core/k_canvas.h"

#include "./llk_ui_elem.h"
#include "./llk_ui_context.h"

struct llk_ui_elem *llk__ui_construct_elem(struct llk_ui_elem *elem, struct llk_ui_context *ui) {

    elem->ui = ui;

    elem->parent = NULL;
    k_list_init(&elem->child_list);
    k_list_node_loop(&elem->sibling_link);

    llk_ui_float_set(&elem->w, 1.0f, LLK_UI_UNIT_PERCENT);
    llk_ui_float_set(&elem->h, 1.0f, LLK_UI_UNIT_PERCENT);

    llk_ui_float_set(&elem->left,   0.0f, LLK_UI_UNIT_NO_VAL);
    llk_ui_float_set(&elem->top,    0.0f, LLK_UI_UNIT_NO_VAL);
    llk_ui_float_set(&elem->right,  0.0f, LLK_UI_UNIT_NO_VAL);
    llk_ui_float_set(&elem->bottom, 0.0f, LLK_UI_UNIT_NO_VAL);

    elem->debug_info = "";

    elem->x = 0.0f;
    elem->y = 0.0f;

    elem->background_color = 0x00000000;
    elem->border_color     = 0x00000000;

    elem->fn_draw = NULL;

    return elem;
}

struct llk_ui_elem *llk_ui_create_elem(struct llk_ui_context *ui) {

    struct llk_ui_elem *elem = llk__ui_mem_alloc(ui, sizeof(struct llk_ui_elem));
    if (NULL == elem)
        return NULL;

    llk__ui_construct_elem(elem, ui);

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

void llk__ui_elem_measure(struct llk_ui_elem *elem) {

    switch (elem->w.unit) {
        case LLK_UI_UNIT_PX:
            elem->w.computed_val = elem->w.specified_val;
            break;
        case LLK_UI_UNIT_PERCENT:
            elem->w.computed_val = elem->parent->w.computed_val * elem->w.specified_val;
            break;
        case LLK_UI_UNIT_VW:
            elem->w.computed_val = elem->ui->vw;
            break;
        case LLK_UI_UNIT_VH:
            elem->w.computed_val = elem->ui->vh;
            break;
        default:
            elem->w.computed_val = 0.0f;
            break;
    }

    switch (elem->h.unit) {
        case LLK_UI_UNIT_PX:
            elem->h.computed_val = elem->h.specified_val;
            break;
        case LLK_UI_UNIT_PERCENT:
            elem->h.computed_val = elem->parent->h.computed_val * elem->h.specified_val;
            break;
        case LLK_UI_UNIT_VW:
            elem->h.computed_val = elem->ui->vw;
            break;
        case LLK_UI_UNIT_VH:
            elem->h.computed_val = elem->ui->vh;
            break;
        default:
            elem->h.computed_val = 0.0f;
            break;
    }

    struct llk_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);

        llk__ui_elem_measure(child);
    }
}

void llk__ui_elem_layout(struct llk_ui_elem *elem) {

    struct llk_ui_elem *parent = elem->parent;

    if (elem->left.unit != LLK_UI_UNIT_NO_VAL) {
        switch (elem->left.unit) {
            case LLK_UI_UNIT_PX:
                elem->left.computed_val = elem->left.specified_val;
                break;
            case LLK_UI_UNIT_PERCENT:
                elem->left.computed_val = parent->w.computed_val * elem->left.specified_val;
                break;
            case LLK_UI_UNIT_VW:
                elem->left.computed_val = elem->ui->vw * elem->left.specified_val;
                break;
            case LLK_UI_UNIT_VH:
                elem->left.computed_val = elem->ui->vh * elem->left.specified_val;
                break;
            default:
                elem->left.computed_val = 0.0f;
                break;
        }

        elem->x = parent->x + elem->left.computed_val;
    }
    else if (elem->right.unit != LLK_UI_UNIT_NO_VAL) {
        switch (elem->right.unit) {
            case LLK_UI_UNIT_PX:
                elem->right.computed_val = elem->right.specified_val;
                break;
            case LLK_UI_UNIT_PERCENT:
                elem->right.computed_val = parent->w.computed_val * elem->right.specified_val;
                break;
            case LLK_UI_UNIT_VW:
                elem->right.computed_val = elem->ui->vw * elem->right.specified_val;
                break;
            case LLK_UI_UNIT_VH:
                elem->right.computed_val = elem->ui->vh * elem->right.specified_val;
                break;
            default:
                elem->right.computed_val = 0.0f;
                break;
        }

        elem->x = parent->x + parent->w.computed_val - elem->w.computed_val - elem->right.computed_val;
    }
    else {
        elem->x = parent->x;
    }

    if (elem->top.unit != LLK_UI_UNIT_NO_VAL) {
        switch (elem->top.unit) {
            case LLK_UI_UNIT_PX:
                elem->top.computed_val = elem->top.specified_val;
                break;
            case LLK_UI_UNIT_PERCENT:
                elem->top.computed_val = parent->h.computed_val * elem->top.specified_val;
                break;
            case LLK_UI_UNIT_VW:
                elem->top.computed_val = elem->ui->vw * elem->top.specified_val;
                break;
            case LLK_UI_UNIT_VH:
                elem->top.computed_val = elem->ui->vh * elem->top.specified_val;
                break;
            default:
                elem->top.computed_val = 0.0f;
                break;
        }

        elem->y = parent->y + elem->top.computed_val;
    }
    else if (elem->bottom.unit != LLK_UI_UNIT_NO_VAL) {
        switch (elem->bottom.unit) {
            case LLK_UI_UNIT_PX:
                elem->bottom.computed_val = elem->bottom.specified_val;
                break;
            case LLK_UI_UNIT_PERCENT:
                elem->bottom.computed_val = parent->h.computed_val * elem->bottom.specified_val;
                break;
            case LLK_UI_UNIT_VW:
                elem->bottom.computed_val = elem->ui->vw * elem->bottom.specified_val;
                break;
            case LLK_UI_UNIT_VH:
                elem->bottom.computed_val = elem->ui->vh * elem->bottom.specified_val;
                break;
            default:
                elem->bottom.computed_val = 0.0f;
                break;
        }

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

    if (elem->fn_draw != NULL) {
        elem->fn_draw(elem);
    }

    struct llk_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);

        llk__ui_elem_draw(child);
    }
}
