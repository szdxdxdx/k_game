#include "./llk_ui_elem_root.h"

static void llk__ui_elem_root_draw(struct llk_ui_elem *elem) {

}

struct llk_ui_elem_type_config llk__ui_elem_root_config = {
    .type_name    = "root",
    .elem_size    = sizeof(struct llk_ui_elem),
    .fn_construct = NULL,
    .fn_destruct  = NULL,
    .fn_draw      = NULL,
};
