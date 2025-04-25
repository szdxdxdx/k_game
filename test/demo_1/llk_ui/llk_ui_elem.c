#include <string.h>
#include <assert.h>

#define K_LOG_TAG "llk:UI"
#include "k_log.h"

#include "./llk_ui_ext.h"

static int llk__ui_elem_set_id(struct llk_ui_elem *elem, const char *val);

/* region [register_type] */

int llk__ui_register_elem_type(struct llk_ui_context *ui, const struct llk_ui_elem_type *type) {

    char *type_name = NULL;

    if (NULL == type) {
        k_log_error("`type` is null");
        goto err;
    } else if (NULL == type->type_name || '\0' == type->type_name[0]) {
        k_log_error("Invalid `config->type_name`");
        goto err;
    } else if (NULL != k_str_map_get(&ui->elem_type_map, type->type_name)) {
        k_log_error("Elem type `%s` already registered", type->type_name);
        goto err;
    }

    type_name = llk__ui_strdup(ui, type->type_name, strlen(type->type_name));
    if (NULL == type_name)
        goto err;

    struct llk_ui_elem_type *copy = k_str_map_add(&ui->elem_type_map, type_name, sizeof(struct llk_ui_elem_type));
    if (NULL == copy) {
        k_log_error("Failed to add type name to registry name map");
        goto err;
    }

    copy->data_size         = type->data_size;
    copy->type_name         = type_name;
    copy->fn_init           = type->fn_init;
    copy->fn_fini           = type->fn_fini;
    copy->fn_set_attr       = type->fn_set_attr;
    copy->fn_after_layout   = type->fn_after_layout;
    copy->fn_hit_test       = type->fn_hit_test;
    copy->fn_dispatch_event = type->fn_dispatch_event;
    copy->fn_draw           = type->fn_draw;

    return 0;

err:
    k_log_error("Failed to register elem type");

    if (NULL != type_name) {
        llk__ui_mem_free(type_name);
    }

    return -1;
}

/* endregion */

/* region [create] */

struct llk_ui_elem *llk_ui_elem_create(struct llk_ui_context *ui, const char *type_name) {

    struct llk_ui_elem *elem = NULL;
    void *data = NULL;

    struct llk_ui_elem_type *type = k_str_map_get(&ui->elem_type_map, type_name);
    if (NULL == type) {
        k_log_error("Unknown UI elem type `%s`", type_name);
        goto err;
    }

    elem = llk__ui_mem_alloc(ui, sizeof(struct llk_ui_elem));
    if (NULL == elem)
        goto err;

    elem->ui = ui;

    elem->parent = NULL;
    k_list_init(&elem->child_list);
    k_list_node_loop(&elem->sibling_link);

    elem->type = type;

    if (0 == type->data_size) {
        elem->data = NULL;
    } else {
        data = llk__ui_mem_alloc(ui, type->data_size);
        if (NULL == data)
            goto err;

        elem->data = data;
    }

    elem->id_map_node.key = "";

    elem->w.unit = LLK_UI_UNIT_NO_VAL;
    elem->h.unit = LLK_UI_UNIT_NO_VAL;

    elem->left.unit   = LLK_UI_UNIT_NO_VAL;
    elem->right.unit  = LLK_UI_UNIT_NO_VAL;
    elem->top.unit    = LLK_UI_UNIT_NO_VAL;
    elem->bottom.unit = LLK_UI_UNIT_NO_VAL;

    elem->x = 0.0f;
    elem->y = 0.0f;

    elem->is_hovered = 0;

    elem->is_hidden = 0;

    elem->flag_destroy = 0;

    if (NULL != type->fn_init) {
        if (0 != type->fn_init(elem))
            goto err;
    }

    return elem;

err:
    k_log_error("Failed to create UI elem `%s`", type_name);
    if (NULL != data) {
        llk__ui_mem_free(data);
    }
    if (NULL != elem) {
        llk__ui_mem_free(elem);
    }
    return NULL;
}

void llk__ui_elem_destroy(struct llk_ui_elem *elem) {

    if (elem->type->fn_fini != NULL) {
        elem->type->fn_fini(elem);
    }

    struct llk_ui_context *ui = elem->ui;

    struct llk_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);

        llk__ui_elem_destroy(child);
    }

    k_list_del(&elem->sibling_link);

    llk__ui_elem_set_id(elem, NULL);

    if (NULL != elem->data) {
        llk__ui_mem_free(elem->data);
    }

    llk__ui_mem_free(elem);

    llk__ui_mark_layout_dirty(ui);
}

void llk_ui_elem_destroy(struct llk_ui_elem *elem) {

    if (NULL == elem)
        return;

    if (NULL == elem->parent) {
        llk_ui_elem_destroy(elem);
        return;
    }

    elem->flag_destroy = 1;
}

static void llk__ui_elem_destroy_if_marked(struct llk_ui_elem *elem) {
    if (elem->flag_destroy) {
        llk__ui_elem_destroy(elem);
    }
}

/* endregion */

/* region [add_child] */

int llk_ui_elem_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child) {

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

/* endregion */

/* region [set_attr] */

static int llk__ui_elem_set_id(struct llk_ui_elem *elem, const char *val) {

    if (NULL == val || '\0' == val[0]) {

        if ('\0' != elem->id_map_node.key[0]) {
            k_str_intrusive_map_del(&elem->id_map_node);

            llk__ui_mem_free((void *)elem->id_map_node.key);
            elem->id_map_node.key = "";
        }
        return 0;
    }

    if (0 == strcmp(val, elem->id_map_node.key))
        return 0;

    if (NULL != k_str_intrusive_map_get(&elem->ui->elem_id_map, val)) {
        k_log_error("UI elem with the same id `%s` already exists", val);
        return -1;
    }

    char *id = llk__ui_strdup(elem->ui, val, strlen(val));
    if (NULL == id)
        return -1;

    if ('\0' != elem->id_map_node.key[0]) {
        k_str_intrusive_map_del(&elem->id_map_node);

        llk__ui_mem_free((void *)elem->id_map_node.key);
    }

    k_str_intrusive_map_add_directly(&elem->ui->elem_id_map, id, &elem->id_map_node);
    return 0;
}

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

static int llk__ui_elem_set_attr_hidden(struct llk_ui_elem *elem, const char *val) {

    if (0 == strcmp(val, "true")) {
        elem->is_hidden = 1;
        return 0;
    }
    else if (0 == strcmp(val, "false")) {
        elem->is_hidden = 0;
        return 0;
    }

    return -1;
}

int llk__ui_elem_set_attr_default(struct llk_ui_elem *elem, const char *key, const char *val) {

    if (0 == strcmp(key, "id"))
        return llk__ui_elem_set_id(elem, val);

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

    if (0 == strcmp(key, "hidden"))
        return llk__ui_elem_set_attr_hidden(elem, val);

    return -1;
}

int llk_ui_elem_set_attr(struct llk_ui_elem *elem, const char *key, const char *val) {

    if (NULL == elem) {
        k_log_warn("elem is null");
        return -1;
    }

    int result;
    if (NULL == elem->type->fn_set_attr) {
        result = llk__ui_elem_set_attr_default(elem, key, val);
    } else {
        result = elem->type->fn_set_attr(elem, key, val);
    }

    if (0 != result) {
        k_log_warn("Elem `%s` unsupported attribute or invalid value, key: `%s` value: `%s`", elem->type->type_name, key, val);
        return -1;
    }

    return 0;
}

/* endregion */

/* region [update && draw] */

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

    llk__ui_elem_destroy_if_marked(elem);

    if (elem->is_hidden)
        return;

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

void llk__ui_elem_layout(struct llk_ui_elem *elem) {

    llk__ui_elem_destroy_if_marked(elem);

    if (elem->is_hidden)
        return;

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

    if (NULL != elem->type->fn_after_layout) {
        elem->type->fn_after_layout(elem);
    }
}

void llk__ui_elem_hit_test(struct llk_ui_elem *elem) {

    llk__ui_elem_destroy_if_marked(elem);

    if (elem->is_hidden)
        return;

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

    if (elem->type->fn_hit_test != NULL) {
        elem->type->fn_hit_test(elem);
    }
}

void llk__ui_elem_dispatch_event(struct llk_ui_elem *elem) {

    llk__ui_elem_destroy_if_marked(elem);

    if (elem->is_hidden)
        return;

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

void llk__ui_elem_draw(struct llk_ui_elem *elem) {

    llk__ui_elem_destroy_if_marked(elem);

    if (elem->is_hidden)
        return;

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

/* region [get] */

int llk_ui_elem_is_hovered(struct llk_ui_elem *elem) {
    return 0 != elem->is_hovered;
}

int llk_ui_elem_is_clicked(struct llk_ui_elem *elem) {
    return elem->is_hovered && elem->ui->mouse_button_down;
}

int llk_ui_elem_is_pressed(struct llk_ui_elem *elem) {
    return elem->is_hovered && elem->ui->mouse_button_pressed;
}

void llk_ui_elem_get_rect(struct llk_ui_elem *elem, float *get_x, float *get_y, float *get_w, float *get_h) {
    llk__ui_layout_if_dirty(elem->ui);
    if (NULL != get_x) { *get_x = elem->x; }
    if (NULL != get_y) { *get_y = elem->y; }
    if (NULL != get_w) { *get_w = elem->w.computed_val; }
    if (NULL != get_h) { *get_h = elem->h.computed_val; }
}

/* endregion */
