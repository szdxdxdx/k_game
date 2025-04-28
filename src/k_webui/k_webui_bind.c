
#define K_LOG_TAG "k_webui:bind"
#include "k_log.h"

#include "./k_webui_context.h"

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

/* region [binding_framework] */

struct k_webui_binding_config {

    size_t data_size;

    int (*fn_init)(void *data, void *param_1, void *param_2);

    void (*fn_unbind)(void *data);

    void (*fn_webui_set)(void *data, const char *val);

    // TODO void (*fn_webui_get)(void *data, const char *req);
};

struct k_webui_binding {

    void *data;

    void (*fn_unbind)(void *data);

    void (*fn_webui_set)(void *data, const char *val);
};

static int k__webui_bind(const char *label, const struct k_webui_binding_config *config, void *param_1, void *param_2) {
    assert(NULL != config);
    assert(NULL != config->fn_webui_set);
    assert(0 < config->data_size);

    if (NULL == label || '\0' == label[0])
        return -1;

    struct k_webui_binding *binding = k_str_map_get(&k__webui.bindings, label);
    if (NULL != binding)
        return -1;

    binding = k_str_map_add(&k__webui.bindings, label, sizeof(struct k_webui_binding) + config->data_size);
    if (NULL == binding)
        return -1;

    binding->data = ptr_offset(binding, sizeof(struct k_webui_binding));
    binding->fn_unbind    = config->fn_unbind;
    binding->fn_webui_set = config->fn_webui_set;

    if (0 != config->fn_init(binding->data, param_1, param_2)) {
        return -1;
    }

    k__webui_exec_js("k__webui.bind({label:%'s,input_type:%'s})", label, "range");

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

static void k__webui_set_binding_C_val(webui_event_t *e) {

    const char *label = webui_get_string_at(e, 0);

    struct k_webui_binding *binding = k_str_map_get(&k__webui.bindings, label);
    if (NULL == binding) {
        k_log_warn("binding not found");
        assert(0);
        return;
    }

    const char *val = webui_get_string_at(e, 1);

    printf("%s, %s\n", label, val);

    binding->fn_webui_set(binding->data, val);
}

int k__webui_binding_init(void) {

    struct k_str_map_options options;
    options.fn_malloc = k__webui_mem_alloc;
    options.fn_free   = k__webui_mem_free;
    if (NULL == k_str_map_construct(&k__webui.bindings, &options))
        return -1;

    webui_bind(k__webui.window, "k__webui_set_binding_C_val", k__webui_set_binding_C_val);

    return 0;
}

void k__webui_binding_fini(void) {
    /* TODO */
}

/* endregion */

/* region [bind_int] */

struct k_webui_int_binding {

    /* 指向绑定的 int 类型变量的指针 */
    int *p_int;

    enum k_webui_bind_input_type input_type;
    union {
        struct k_webui_bind_int_range    range;
        struct k_webui_bind_int_checkbox checkbox;
    };
};

static int k__webui_int_binding_init(void *data, void *param_1, void *param_2) {
    struct k_webui_int_binding *int_binding = data;
    int *p_int = param_1;
    const struct k_webui_bind_int_options *options = param_2;

    int_binding->p_int = p_int;

    return 0;
}

static void k__webui_set_int(void *data, const char *val) {
    struct k_webui_int_binding *int_binding = data;

    int new_val = atoi(val);
    *int_binding->p_int = new_val;
}

void k_webui_bind_int(const char *label, int *p_int, const struct k_webui_bind_int_options *options) {

    struct k_webui_binding_config config;
    config.data_size    = sizeof(struct k_webui_int_binding);
    config.fn_init      = k__webui_int_binding_init;
    config.fn_unbind    = NULL;
    config.fn_webui_set = k__webui_set_int;

    k__webui_bind(label, &config, p_int, (void *)options);
}

/* endregion */
