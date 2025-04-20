#include <string.h>

#include "k_log.h"

#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"

static int check_config(struct llk_ui_context *ui, const struct llk_ui_elem_type_config *config) {

    if (NULL == config) {
        k_log_error("invalid `config`");
        return -1;
    }

    if (config->elem_size < sizeof(struct llk_ui_elem)) {
        k_log_error("invalid `config->elem_size`", config->type_name);
    }

    if (NULL != k_str_map_get(&ui->elem_type_map, config->type_name)) {
        k_log_error("type `%s` already registered", config->type_name);
        return -1;
    }

    return 0;
}

int llk_ui_register_elem_type(struct llk_ui_context *ui, const struct llk_ui_elem_type_config *config) {

    if (0 != check_config(ui, config))
        goto err;

    size_t val_size = sizeof(struct llk_ui_elem_type_info) + strlen(config->type_name) + 1;

    struct llk_ui_elem_type_info *type_info = k_str_map_add(&ui->elem_type_map, config->type_name, val_size);
    if (NULL == type_info)
        goto err;

    char *type_name = (char *)type_info + sizeof(struct llk_ui_elem_type_info);
    strcpy(type_name, config->type_name);

    type_info->elem_size    = config->elem_size;
    type_info->type_name    = type_name;
    type_info->fn_construct = config->fn_construct;
    type_info->fn_destruct  = config->fn_destruct;
    type_info->fn_draw      = config->fn_draw;

    return 0;

err:
    k_log_error("Failed to register llk UI element type");
    return -1;
}
