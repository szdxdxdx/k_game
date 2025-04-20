#include "k_log.h"

#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"
#include "./llk_ui_elem_type_registry.h"

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

    if (NULL != type_info->fn_init) {
        if (0 != type_info->fn_init(elem))
            return NULL;
    }

    return elem;
}

struct llk_ui_elem *llk_ui_create_elem(struct llk_ui_context *ui, const char *type_name) {

    struct llk_ui_elem_type_info *type_info = k_str_map_get(&ui->elem_type_map, type_name);
    if (NULL == type_info) {
        k_log_error("llk UI element type `%s` not registered", type_name);
        return NULL;
    }

    size_t alloc_size = type_info->data_size + sizeof(struct llk_ui_elem);

    struct llk_ui_elem *elem = llk__ui_mem_alloc(ui, alloc_size);
    if (NULL == elem)
        return NULL;

    elem->data = (void *)((char *)elem + sizeof(struct llk_ui_elem));

    if (NULL == llk__ui_construct_elem(elem, ui, type_info)) {
        llk__ui_mem_free(elem);
        return NULL;
    }

    return elem;
}

void llk__ui_destruct_elem(struct llk_ui_elem *elem) {

    struct llk_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);

        llk__ui_destruct_elem(child);
    }

    if (elem->type->fn_fini != NULL) {
        elem->type->fn_fini(elem);
    }
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
