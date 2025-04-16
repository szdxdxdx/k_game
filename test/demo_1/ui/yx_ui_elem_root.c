
#include "./yx_ui_elem_root.h"
#include "./yx_ui_elem.h"
#include "yx_ui_context.h"

struct yx_ui_elem *yx_ui_elem_root_create(struct yx_ui_context *ui) {

    struct yx_ui_elem *elem = yx__ui_mem_alloc(ui, sizeof(struct yx_ui_elem));

    return elem;
}
