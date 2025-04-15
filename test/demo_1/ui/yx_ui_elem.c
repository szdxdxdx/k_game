#include <string.h>

#include "./yx_ui_ext.h"
#include "./yx_ui_context.h"
#include "./yx_ui_elem.h"


static void yx__ui_elem_default_fn_draw(struct yx_ui_elem *elem) {
    (void)elem;
}

static int yx__ui_elem_default_fn_set_attr(struct yx_ui_elem *elem, const char *key, const char *val) {
    (void)elem;
    (void)key;
    (void)val;
    return -1;
}

struct yx_ui_elem *yx__ui_elem_create(struct yx_ui_context *ui, size_t data_size) {

    struct yx_ui_elem *elem = yx__ui_mem_alloc(ui, sizeof(struct yx_ui_elem));
    if (NULL == elem)
        return NULL;

    void *data = NULL;

    if (0 < data_size) {
        data = yx__ui_mem_alloc(ui, data_size);
        if (NULL == data) {
            yx__ui_mem_free(ui, elem);
            return NULL;
        }
    }

    elem->sibling_node.prev = NULL;
    elem->sibling_node.next = NULL;

    k_list_init(&elem->child_list);

    elem->ui = ui;

    elem->fn_draw     = yx__ui_elem_default_fn_draw;
    elem->fn_set_attr = yx__ui_elem_default_fn_set_attr;

    return elem;
}

int yx_ui_set_attr(struct yx_ui_elem *elem, const char *key, const char *val) {
    return elem->fn_set_attr(elem, key, val);
}

int yx_ui_append_child(struct yx_ui_elem *elem, struct yx_ui_elem *child) {

    if (NULL == elem || NULL == child)
        return -1;
    if (elem->ui != child->ui)
        return -1;

    k_list_add_tail(&elem->child_list, &child->sibling_node);
    return 0;
}

void yx__ui_elem_draw(struct yx_ui_elem *elem) {

    elem->fn_draw(elem);

    struct yx_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct yx_ui_elem, sibling_node);

        yx__ui_elem_draw(child);
    }
}
