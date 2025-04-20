#include "./llk_ui_elem_type_builtin.h"
#include "llk_ui_elem_type_registry.h"

struct llk_ui_elem_type_config llk__ui_elem_root_config = {
    .type_name = "root",
    .data_size = sizeof(struct llk_ui_elem),
    .fn_init   = NULL,
    .fn_fini   = NULL,
    .fn_draw   = NULL,
};
