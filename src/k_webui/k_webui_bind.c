#include "k_str_buf.h"

#define K_LOG_TAG "k_webui:bind"
#include "k_log.h"

#include "./k_webui_internal.h"

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

struct k_webui_binding_config {

    const char *label;

    size_t data_size;

    int (*fn_init)(void *data, void *param_1, void *param_2);

    void (*fn_unbind)(void *data);

    void (*fn_webui_set)(void *data, const char *val);

    enum k_webui_input_type input_type;
    union {
        struct k_webui_int_range   int_range;
        struct k_webui_checkbox    checkbox;
        struct k_webui_float_range float_range;
        struct k_webui_button      button;
    };
};

struct k_webui_binding {

    void *data;

    void (*fn_unbind)(void *data);

    void (*fn_webui_set)(void *data, const char *val);
};

static void k__webui_bind_exec_js(const struct k_webui_binding_config *config) {
    char buf[512];
    struct k_str_buf str_buf;
    k_str_buf_init(&str_buf, buf, sizeof(buf));

    k_str_buf_puts(&str_buf, "k__webui.bind({");

    k_str_buf_k_printf(&str_buf, k__webui_fmt, "label:%'s,", config->label);

    k_str_buf_puts(&str_buf, "input:{");
    {
        switch (config->input_type) {
            case K_WEBUI_INT_RANGE:
                k_str_buf_printf(&str_buf, "type:'%s',", "range");
                k_str_buf_printf(&str_buf, "min:%d,",  config->int_range.min);
                k_str_buf_printf(&str_buf, "max:%d,",  config->int_range.max);
                k_str_buf_printf(&str_buf, "step:%d,", config->int_range.step);
                break;
            case K_WEBUI_FLOAT_RANGE:
                k_str_buf_printf(&str_buf, "type:'%s',", "range");
                k_str_buf_printf(&str_buf, "min:%.3f,",  config->float_range.min);
                k_str_buf_printf(&str_buf, "max:%.3f,",  config->float_range.max);
                k_str_buf_printf(&str_buf, "step:%.3f,", config->float_range.step);
                break;
            case K_WEBUI_CHECKBOX:
                k_str_buf_printf(&str_buf, "type:'%s',", "checkbox");
                break;
            case K_WEBUI_BUTTON:
                k_str_buf_printf(&str_buf, "type:'%s',", "button");
                break;
        }
    }
    k_str_buf_puts(&str_buf, "},");

    k_str_buf_puts(&str_buf, "})");

    char *js = k_str_buf_get(&str_buf);
    k__webui_exec_js(js);

    k_str_buf_free(&str_buf);
}

static int k__webui_bind(const struct k_webui_binding_config *config, void *param_1, void *param_2) {
    assert(NULL != config);
    assert(NULL != config->fn_webui_set);
    assert(0 < config->data_size);

    if (NULL == config)
        return -1;
    if (NULL == config->label || '\0' == config->label[0])
        return -1;

    struct k_webui_binding *binding = k_str_map_get(&k__webui.bindings, config->label);
    if (NULL != binding)
        return -1;

    binding = k_str_map_add(&k__webui.bindings, config->label, sizeof(struct k_webui_binding) + config->data_size);
    if (NULL == binding)
        return -1;

    binding->data = ptr_offset(binding, sizeof(struct k_webui_binding));
    binding->fn_unbind    = config->fn_unbind;
    binding->fn_webui_set = config->fn_webui_set;

    if (0 != config->fn_init(binding->data, param_1, param_2)) {
        return -1;
    }

    k__webui_bind_exec_js(config);
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

/* region [bind_int] */

struct k_webui_int_binding {

    /* 指向绑定的 int 类型变量的指针 */
    int *p_int;
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

int k_webui_bind_int(const char *label, int *p_int, const struct k_webui_int_options *options) {

    struct k_webui_binding_config config;
    config.label        = label;
    config.data_size    = sizeof(struct k_webui_int_binding);
    config.fn_init      = k__webui_int_binding_init;
    config.fn_unbind    = NULL;
    config.fn_webui_set = k__webui_set_int;

    if (NULL == options) {
        config.input_type = K_WEBUI_INT_RANGE;
        config.int_range.min  = -100;
        config.int_range.max  = 100;
        config.int_range.step = 1;
    }
    else {
        switch (options->input_type) {
            case K_WEBUI_INT_RANGE:
                config.input_type = K_WEBUI_INT_RANGE;
                config.int_range.min  = options->range.min;
                config.int_range.max  = options->range.max;
                config.int_range.step = options->range.step;
                break;
            case K_WEBUI_CHECKBOX:
                break;
            default:
                return -1;
        }
    }

    return k__webui_bind(&config, p_int, (void *)options);
}

/* endregion */
