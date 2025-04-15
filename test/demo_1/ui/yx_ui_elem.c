#include <assert.h>

#include "./yx_ui_ext.h"
#include "./yx_ui_context.h"
#include "./yx_ui_elem.h"

static void default_fn_destruct(struct yx_ui_elem *elem) {
    (void)elem;
}

static void default_fn_draw(struct yx_ui_elem *elem) {
    (void)elem;
}

static int default_fn_set_attr(struct yx_ui_elem *elem, const char *key, const char *val) {
    (void)elem;
    (void)key;
    (void)val;
    return -1;
}

int yx__ui_elem_construct(struct yx_ui_elem *elem, struct yx_ui_context *ui) {
    assert(NULL != elem);
    assert(NULL != ui);

    if (NULL == ui || NULL == elem)
        return -1;

    k_list_node_loop(&elem->sibling_link);
    k_list_init(&elem->child_list);

    elem->ui = ui;

    static struct yx_ui_elem_v_tbl v_tbl = {
        .fn_draw     = default_fn_draw,
        .fn_set_attr = default_fn_set_attr
    };
    elem->v_tbl = &v_tbl;

    return 0;
}

void yx__ui_elem_destruct(struct yx_ui_elem *elem) {
    assert(NULL != elem);
    k_list_del(&elem->sibling_link);
}

int yx_ui_set_attr(struct yx_ui_elem *elem, const char *key, const char *val) {
    return elem->v_tbl->fn_set_attr(elem, key, val);
}

int yx_ui_append_child(struct yx_ui_elem *elem, struct yx_ui_elem *child) {

    if (NULL == elem || NULL == child)
        return -1;
    if (elem->ui != child->ui)
        return -1;

    k_list_add_tail(&elem->child_list, &child->sibling_link);
    return 0;
}

void yx__ui_elem_draw(struct yx_ui_elem *elem) {

    elem->v_tbl->fn_draw(elem);

    struct yx_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct yx_ui_elem, sibling_link);

        yx__ui_elem_draw(child);
    }
}
