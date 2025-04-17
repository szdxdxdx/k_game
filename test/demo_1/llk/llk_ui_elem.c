#include "k_game/core/k_canvas.h"

#include "./llk_ui_elem.h"
#include "./llk_ui_context.h"

struct llk_ui_elem *llk_ui_construct_elem(struct llk_ui_elem *elem, struct llk_ui_context *ui) {

    elem->elem_type_name = "";
    elem->ui = ui;

    elem->parent = NULL;
    llk_ui_elem_array_construct(&elem->children);

    elem->w.unit = LLK_UI_UNIT_PX;
    elem->w.val  = 0.0f;

    elem->h.unit = LLK_UI_UNIT_PX;
    elem->h.val  = 0.0f;

    elem->background_color = 0x00000000;

    elem->fn_measure = NULL;
    elem->fn_layout  = NULL;
    elem->fn_paint   = NULL;

    return elem;
}

struct llk_ui_elem *llk_ui_create_elem(struct llk_ui_context *ui) {

    struct llk_ui_elem *elem = malloc(sizeof(struct llk_ui_elem));
    if (NULL == elem)
        return NULL;

    return llk_ui_construct_elem(elem, ui);
}

int llk_ui_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child) {

    if (NULL == parent || NULL == child)
        return -1;
    if (NULL != child->parent)
        return -1;
    if (child->ui != parent->ui)
        return -1;

    if (0 != llk_ui_elem_array_push_back(&parent->children, child))
        return -1;

    child->parent = parent;
    return 0;
}

void llk_ui_elem_measure(struct llk_ui_elem *elem) {

    switch (elem->w.unit) {
        case LLK_UI_UNIT_PX:
            elem->w.computed_val = elem->w.val;
            break;
        case LLK_UI_UNIT_PERCENT:
            elem->w.computed_val = elem->parent->w.computed_val * elem->w.val;
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
            elem->h.computed_val = elem->h.val;
            break;
        case LLK_UI_UNIT_PERCENT:
            elem->h.computed_val = elem->parent->h.computed_val * elem->h.val;
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

    size_t i = 0;
    for (; i < elem->children.size; i++) {
        struct llk_ui_elem *child = elem->children.storage[i];
        llk_ui_elem_measure(child);
    }
}

void llk_ui_elem_layout(struct llk_ui_elem *elem) {

    struct llk_ui_elem *parent = elem->parent;

    if (elem->left.unit != LLK_UI_UNIT_NO_VAL) {
        switch (elem->left.unit) {
            case LLK_UI_UNIT_PX:
                elem->left.computed_val = elem->left.val;
                break;
            case LLK_UI_UNIT_PERCENT:
                elem->left.computed_val = parent->w.computed_val * elem->left.val;
                break;
            case LLK_UI_UNIT_VW:
                elem->left.computed_val = elem->ui->vw * elem->left.val;
                break;
            case LLK_UI_UNIT_VH:
                elem->left.computed_val = elem->ui->vh * elem->left.val;
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
                elem->right.computed_val = elem->right.val;
                break;
            case LLK_UI_UNIT_PERCENT:
                elem->right.computed_val = parent->w.computed_val * elem->right.val;
                break;
            case LLK_UI_UNIT_VW:
                elem->right.computed_val = elem->ui->vw * elem->right.val;
                break;
            case LLK_UI_UNIT_VH:
                elem->right.computed_val = elem->ui->vh * elem->right.val;
                break;
            default:
                elem->right.computed_val = 0.0f;
                break;
        }

        elem->x = parent->x + parent->w.computed_val - elem->right.computed_val - elem->w.computed_val;
    }
    else {
        elem->x = parent->x;
    }

    if (elem->top.unit != LLK_UI_UNIT_NO_VAL) {
        switch (elem->top.unit) {
            case LLK_UI_UNIT_PX:
                elem->top.computed_val = elem->top.val;
                break;
            case LLK_UI_UNIT_PERCENT:
                elem->top.computed_val = parent->h.computed_val * elem->top.val;
                break;
            case LLK_UI_UNIT_VW:
                elem->top.computed_val = elem->ui->vw * elem->top.val;
                break;
            case LLK_UI_UNIT_VH:
                elem->top.computed_val = elem->ui->vh * elem->top.val;
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
                elem->bottom.computed_val = elem->bottom.val;
                break;
            case LLK_UI_UNIT_PERCENT:
                elem->bottom.computed_val = parent->h.computed_val * elem->bottom.val;
                break;
            case LLK_UI_UNIT_VW:
                elem->bottom.computed_val = elem->ui->vw * elem->bottom.val;
                break;
            case LLK_UI_UNIT_VH:
                elem->bottom.computed_val = elem->ui->vh * elem->bottom.val;
                break;
            default:
                elem->bottom.computed_val = 0.0f;
                break;
        }
    }
    else {
        elem->y = parent->y;
    }

    size_t i = 0;
    for (; i < elem->children.size; i++) {
        struct llk_ui_elem *child = elem->children.storage[i];
        llk_ui_elem_layout(child);
    }
}

void llk_ui_elem_paint(struct llk_ui_elem *elem) {

    k_canvas_set_draw_color_rgba(elem->background_color);
    k_canvas_fill_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);

    size_t i = 0;
    for (; i < elem->children.size; i++) {
        struct llk_ui_elem *child = elem->children.storage[i];
        llk_ui_elem_paint(child);
    }
}
