#include "k_log.h"

#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"

static int check_config(struct llk_ui_context *ui, const struct llk_ui_elem_type_config *config) {

    if (NULL == config) {
        k_log_error("invalid `config`");
        return -1;
    }

    if (config->elem_size < sizeof(struct llk_ui_elem)) {
        k_log_error("invalid `config->elem_size`", config->elem_type_name);
    }

    if (NULL != k_str_map_get(&ui->elem_type_map, config->elem_type_name)) {
        k_log_error("type `%s` already registered", config->elem_type_name);
        return -1;
    }

    if (NULL == config->fn_construct) {
        k_log_error("invalid `config->fn_construct`");
        return -1;
    }

    if (NULL == config->fn_destruct) {
        k_log_error("invalid `config->fn_destruct`");
        return -1;
    }

    return 0;
}

int llk_ui_register_elem_type(struct llk_ui_context *ui, const struct llk_ui_elem_type_config *config) {

    if (0 != check_config(ui, config)) {
        k_log_error("Failed to register llk UI element type");
        return -1;
    }





    return 0;
}
