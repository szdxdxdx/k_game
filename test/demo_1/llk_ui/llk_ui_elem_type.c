#include <string.h>

#include "k_log.h"

#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"
#include "./llk_ui_elem_type.h"

static int check_config(struct llk_ui_context *ui, const struct llk_ui_elem_type_config *config) {

    if (NULL == config) {
        k_log_error("llk UI element: invalid `config`");
        return -1;
    }

    if (NULL == config->type_name || '\0' == config->type_name[0]) {
        k_log_error("llk UI element: invalid `config->type_name`");
        return -1;
    }

    if (NULL != k_str_map_get(&ui->elem_type_map, config->type_name)) {
        k_log_error("llk UI element: type `%s` already registered", config->type_name);
        return -1;
    }

    return 0;
}

int llk_ui_register_elem_type(struct llk_ui_context *ui, const struct llk_ui_elem_type_config *config) {

    if (0 != check_config(ui, config))
        goto err;

    char *type_name = llk__ui_mem_alloc(ui, strlen(config->type_name) + 1);
    if (NULL == type_name)
        goto err;

    strcpy(type_name, config->type_name);

    struct llk_ui_elem_type *type = k_str_map_add(&ui->elem_type_map, type_name, sizeof(struct llk_ui_elem_type));
    if (NULL == type) {
        llk__ui_mem_free(type_name);
        k_log_error("llk UI element: Failed to add element type name to registry name map");
        goto err;
    }

    type->data_size   = config->data_size;
    type->type_name   = type_name;
    type->fn_init     = config->fn_init;
    type->fn_fini     = config->fn_fini;
    type->fn_set_attr = config->fn_set_attr;
    type->fn_draw     = config->fn_draw;

    return 0;

err:
    k_log_error("llk UI element: Failed to register element type");
    return -1;
}
