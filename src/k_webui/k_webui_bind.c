
#include "./k_webui_context.h"

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

int k__webui_binding_init(void) {

    struct k_str_map_options options;
    options.fn_malloc = k__webui_mem_alloc;
    options.fn_free   = k__webui_mem_free;

    struct k_str_map *groups = k_str_map_construct(&k__webui.bindings, &options);
    if (NULL == groups)
        return -1;

    /* ... */

    return 0;
}

void k__webui_binding_fini(void) {
    /* TODO */
}

int k_webui_bind(const char *label, const struct k_webui_binding_config *config, void *params) {

    if (NULL == config)
        return -1;
    if (NULL == label || '\0' == label[0])
        return -1;
    if (NULL == config->fn_webui_set)
        return -1;

    struct k_webui_binding *binding = k_str_map_get(&k__webui.bindings, label);
    if (NULL != binding)
        return -1;

    binding = k_str_map_add(&k__webui.bindings, label, sizeof(struct k_webui_binding) + config->data_size);
    if (NULL == binding)
        return -1;

    binding->data = ptr_offset(binding, sizeof(struct k_webui_binding));
    binding->fn_unbind = config->fn_unbind;

    if (0 != config->fn_init(binding->data, params)) {
        return -1;
    }

    return 0;
}

void k_webui_unbind(const char *label) {

    struct k_webui_binding *binding = k_str_map_get(&k__webui.bindings, label);
    if (NULL == binding)
        return;

    if (binding->fn_unbind != NULL) {
        binding->fn_unbind(binding->data);
    }

    k_str_map_remove(&k__webui.bindings, label);
}
