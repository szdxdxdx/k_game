#include <string.h>

#include "k_log.h"

#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"
#include "./llk_ui_elem_type.h"
#include "./llk_ui_mem_alloc.h"
#include "./llk_ui_val_parser.h"

/* region [register_type] */

int llk_ui_register_elem_type(struct llk_ui_context *ui, const struct llk_ui_elem_type_config *config) {

    char *type_name = NULL;

    if (NULL == config) {
        k_log_error("llk UI: invalid `config`");
        goto err;
    } else if (NULL == config->type_name || '\0' == config->type_name[0]) {
        k_log_error("llk UI: invalid `config->type_name`");
        goto err;
    } else if (NULL != k_str_map_get(&ui->elem_type_map, config->type_name)) {
        k_log_error("llk UI: type `%s` already registered", config->type_name);
        goto err;
    }

    type_name = llk__ui_mem_alloc(ui, strlen(config->type_name) + 1);
    if (NULL == type_name)
        goto err;

    strcpy(type_name, config->type_name);

    struct llk_ui_elem_type *type = k_str_map_add(&ui->elem_type_map, type_name, sizeof(struct llk_ui_elem_type));
    if (NULL == type) {
        k_log_error("llk UI: failed to add type name to registry name map");
        goto err;
    }

    type->data_size         = config->data_size;
    type->type_name         = type_name;
    type->fn_init           = config->fn_init;
    type->fn_fini           = config->fn_fini;
    type->fn_set_attr       = config->fn_set_attr;
    type->fn_draw           = config->fn_draw;
    type->fn_dispatch_event = config->fn_dispatch_event;

    return 0;

err:
    k_log_error("llk UI: failed to register element type");

    if (NULL != type_name) {
        llk__ui_mem_free(type_name);
    }

    return -1;
}

/* endregion */

/* region [create] */

struct llk_ui_elem *llk__ui_construct_elem(struct llk_ui_elem *elem, struct llk_ui_context *ui, const struct llk_ui_elem_type *type) {

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

    elem->type = type;

    if (NULL != type->fn_init) {
        if (0 != type->fn_init(elem))
            return NULL;
    }

    return elem;
}

struct llk_ui_elem *llk_ui_create_elem(struct llk_ui_context *ui, const char *type_name) {

    struct llk_ui_elem_type *type = k_str_map_get(&ui->elem_type_map, type_name);
    if (NULL == type) {
        k_log_error("llk UI: elem type `%s` not registered", type_name);
        return NULL;
    }

    struct llk_ui_elem *elem = llk__ui_mem_alloc(ui, sizeof(struct llk_ui_elem));
    if (NULL == elem)
        return NULL;

    if (0 == type->data_size) {
        elem->data = NULL;
    } else {
        elem->data = llk__ui_mem_alloc(ui, type->data_size);
        if (NULL == elem->data) {
            llk__ui_mem_free(elem);
        }
    }

    if (NULL == llk__ui_construct_elem(elem, ui, type)) {
        llk__ui_mem_free(elem->data);
        llk__ui_mem_free(elem);
        return NULL;
    }

    return elem;
}

void llk__ui_destruct_elem(struct llk_ui_elem *elem) {

    if (elem->type->fn_fini != NULL) {
        elem->type->fn_fini(elem);
    }

    struct llk_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);

        llk__ui_destruct_elem(child);
    }
}

/* endregion */

/* region [add_child] */

int llk_ui_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child) {

    /* TODO
     * 添加元素可能发生在事件派发过程，所以这里应只做标记，等事件派发完毕之后再统一添加
     */

    if (NULL == parent || NULL == child)
        return -1;
    if (NULL != child->parent)
        return -1;
    if (child->ui != parent->ui)
        return -1;

    k_list_add_tail(&parent->child_list, &child->sibling_link);
    child->parent = parent;

    llk__ui_mark_layout_dirty(parent->ui);

    return 0;
}

void llk_ui_elem_remove(struct llk_ui_elem *elem) {
    /* TODO
     * 删除元素可能发生再事件派发过程，所以这里只做标记，等事件派发完毕之后再统一删除
     */
}

/* endregion */

/* region [set_attr] */

/* region [set_size] */

static int llk__ui_elem_set_attr_w(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    if (float_val < 0.0f)
        return -1;

    elem->w.specified_val = float_val;
    elem->w.unit = unit;

    return 0;
}

static int llk__ui_elem_set_attr_h(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    if (float_val < 0.0f)
        return -1;

    elem->h.specified_val = float_val;
    elem->h.unit = unit;

    return 0;
}

/* endregion */

/* region [set_position] */

static int llk__ui_elem_set_attr_left(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    elem->left.specified_val = float_val;
    elem->left.unit = unit;

    return 0;
}

static int llk__ui_elem_set_attr_right(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    elem->right.specified_val = float_val;
    elem->right.unit = unit;

    return 0;
}

static int llk__ui_elem_set_attr_top(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    elem->top.specified_val = float_val;
    elem->top.unit = unit;

    return 0;
}

static int llk__ui_elem_set_attr_bottom(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    elem->bottom.specified_val = float_val;
    elem->bottom.unit = unit;

    return 0;
}

/* endregion */

int llk__ui_elem_set_attr_default(struct llk_ui_elem *elem, const char *key, const char *val) {

    if (0 == strcmp(key, "w"))
        return llk__ui_elem_set_attr_w(elem, val);
    if (0 == strcmp(key, "h"))
        return llk__ui_elem_set_attr_h(elem, val);

    if (0 == strcmp(key, "left"))
        return llk__ui_elem_set_attr_left(elem, val);
    if (0 == strcmp(key, "right"))
        return llk__ui_elem_set_attr_right(elem, val);
    if (0 == strcmp(key, "top"))
        return llk__ui_elem_set_attr_top(elem, val);
    if (0 == strcmp(key, "bottom"))
        return llk__ui_elem_set_attr_bottom(elem, val);

    return -1;
}

int llk_ui_elem_set_attr(struct llk_ui_elem *elem, const char *key, const char *val) {

    int result;
    if (NULL == elem->type->fn_set_attr) {
        result = llk__ui_elem_set_attr_default(elem, key, val);
    } else {
        result = elem->type->fn_set_attr(elem, key, val);
    }

    if (0 != result) {
        k_log_warn("llk UI: elem `%s` unsupported attribute or invalid value, key: `%s` value: `%s`", elem->type->type_name, key, val);
        return -1;
    }

    return 0;
}

/* endregion */

/* region [update && draw] */

/* region [measure] */

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

/* endregion */

/* region [layout] */

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

/* endregion */

/* region [hit_test] */

void llk__ui_elem_hit_test(struct llk_ui_elem *elem) {

    struct llk_ui_context *ui = elem->ui;

    float x = elem->x;
    float y = elem->y;
    float w = elem->w.computed_val;
    float h = elem->h.computed_val;
    float mouse_x = ui->mouse_x;
    float mouse_y = ui->mouse_y;
    if (x <= mouse_x && mouse_x <= x + w && y <= mouse_y && mouse_y <= y + h) {
        elem->is_hovered = 1;
    } else {
        elem->is_hovered = 0;
    }

    struct llk_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);

        llk__ui_elem_hit_test(child);
    }
}

/* endregion */

/* region [dispatch_event] */

void llk__ui_elem_dispatch_event(struct llk_ui_elem *elem) {

    struct llk_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);

        llk__ui_elem_dispatch_event(child);
    }

    if (elem->type->fn_dispatch_event != NULL) {
        elem->type->fn_dispatch_event(elem);
    }
}

/* endregion */

/* region [draw] */

void llk__ui_elem_draw(struct llk_ui_elem *elem) {

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

/* endregion */

/* endregion */
