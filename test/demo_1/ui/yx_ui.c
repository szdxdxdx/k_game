#include <string.h>

#include "k_list.h"

#include "k_game/core/k_object.h"

/* region [ui_context] */

struct yx_ui {
    struct k_object *object;

    struct yx_ui_elem *body;
};

struct yx_ui *yx_ui_create_context(void) {

    struct k_object *object = k_object_create(sizeof(struct yx_ui));
    if (NULL == object)
        return NULL;

    struct yx_ui *ui = k_object_get_data(object);
    ui->object = object;

    return ui;
}

struct yx_ui_elem *yx_ui_get_body(struct yx_ui *ui) {
    return ui->body;
}

/* endregion */

/* region [ui_elem_base] */

struct yx_ui_elem {

    struct k_list_node sibling_node;

    struct k_list child_list;
};

/* endregion */

/* region [create_elem] */

struct yx_ui_elem *yx_ui_create_button(void);

struct yx_ui_elem *yx_ui_create_elem(const char *elem_type) {

    if (0 == strncmp(elem_type, "button", 6)) {
        return yx_ui_create_button();
    }

    return NULL;
}

/* endregion */

/* region [button] */

struct yx_ui_elem *yx_ui_create_button(void) {


}

/* endregion */
