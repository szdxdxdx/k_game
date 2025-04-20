#include "./llk_ui_elem.h"
#include "./llk_ui_context.h"

static void llk__ui_elem_compute_edge_offset(struct llk_ui_elem *elem) {

    struct llk_ui_elem *parent = elem->parent;

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
}

static void llk__ui_elem_compute_size(struct llk_ui_elem *elem) {

    struct llk_ui_elem *parent = elem->parent;

    switch (elem->w.unit) {
        case LLK_UI_UNIT_NO_VAL:
            if (elem->left.unit != LLK_UI_UNIT_NO_VAL && elem->right.unit != LLK_UI_UNIT_NO_VAL) {
                elem->w.computed_val = parent->w.computed_val - elem->left.computed_val - elem->right.computed_val;
                if (elem->w.computed_val < 0.0f) {
                    elem->w.computed_val = 0.0f;
                }
            } else {
                elem->w.computed_val = 0.0f;
            }
            break;
        case LLK_UI_UNIT_PX:
            elem->w.computed_val = elem->w.specified_val;
            break;
        case LLK_UI_UNIT_PERCENT:
            elem->w.computed_val = parent->w.computed_val * elem->w.specified_val;
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
        case LLK_UI_UNIT_NO_VAL:
            if (elem->top.unit != LLK_UI_UNIT_NO_VAL && elem->bottom.unit != LLK_UI_UNIT_NO_VAL) {
                elem->h.computed_val = parent->h.computed_val - elem->top.computed_val - elem->bottom.computed_val;
                if (elem->h.computed_val < 0.0f) {
                    elem->h.computed_val = 0.0f;
                }
            } else {
                elem->h.computed_val = 0.0f;
            }
            break;
        case LLK_UI_UNIT_PX:
            elem->h.computed_val = elem->h.specified_val;
            break;
        case LLK_UI_UNIT_PERCENT:
            elem->h.computed_val = parent->h.computed_val * elem->h.specified_val;
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
}

void llk__ui_elem_measure(struct llk_ui_elem *elem) {

    llk__ui_elem_compute_edge_offset(elem);

    llk__ui_elem_compute_size(elem);

    struct llk_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);

        llk__ui_elem_measure(child);
    }
}
