#include "k_str_buf.h"

#define K_LOG_TAG "k_webui:bind"
#include "k_log.h"

#include "./k_webui_internal.h"

/* region [utils] */

static int parse_int(const char *str, int *get_val) {

    errno = 0;
    char *end;
    long val = strtol(str, &end, 10);

    if (errno == ERANGE)
        return -1;
    if (val > INT_MAX || val < INT_MIN)
        return -1;
    if (end == str)
        return -1;

    *get_val = (int)val;
    return 0;
}

static int parse_float(const char *str, float *get_val) {

    errno = 0;
    char *end;
    float val = strtof(str, &end);

    if (str == end)
        return -1;
    if (errno == ERANGE)
        return -1;

    *get_val = val;
    return 0;
}

/* endregion */

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

/* region [init] */

static void k__webui_js_set_C_val(webui_event_t *e);

int k__webui_binding_init(void) {

    struct k_str_map_options options;
    options.fn_malloc = k__webui_mem_alloc;
    options.fn_free   = k__webui_mem_free;
    if (NULL == k_str_map_construct(&k__webui.bindings, &options))
        return -1;

    webui_bind(k__webui.window, "k__webui_js_set_C_val", k__webui_js_set_C_val);

    return 0;
}

void k__webui_binding_fini(void) {
    /* TODO */
}

/* endregion */

/* region [UI_widget] */

struct k_webui_widget_config {
    enum k_webui_input_type input_type;
    union {
        struct k_webui_int_range   int_range;
        struct k_webui_checkbox    checkbox;
        struct k_webui_float_range float_range;
        struct k_webui_button      button;
    };
};

static void k__webui_exec_js_add_widget(const char *label, const struct k_webui_widget_config *widget) {

    struct k_str_buf str_buf;
    char buf[1024];
    k_str_buf_init(&str_buf, NULL, sizeof(buf));

    k_str_buf_puts(&str_buf, "k__webui.bind({");
    {
        k_str_buf_k_printf(&str_buf, k__webui_fmt, "label:%'s,", label);

        k_str_buf_puts(&str_buf, "input:{");
        {
            switch (widget->input_type) {
                case K_WEBUI_INT_RANGE:
                    k_str_buf_printf(&str_buf, "type:'%s',", "range");
                    k_str_buf_printf(&str_buf, "min:%d,",  widget->int_range.min);
                    k_str_buf_printf(&str_buf, "max:%d,",  widget->int_range.max);
                    k_str_buf_printf(&str_buf, "step:%d,", widget->int_range.step);
                    break;
                case K_WEBUI_FLOAT_RANGE:
                    k_str_buf_printf(&str_buf, "type:'%s',", "range");
                    k_str_buf_printf(&str_buf, "min:%.3f,",  widget->float_range.min);
                    k_str_buf_printf(&str_buf, "max:%.3f,",  widget->float_range.max);
                    k_str_buf_printf(&str_buf, "step:%.3f,", widget->float_range.step);
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
    }
    k_str_buf_puts(&str_buf, "})");

    char *js = k_str_buf_get(&str_buf);
    k__webui_exec_js(js);

    k_str_buf_free(&str_buf);
}

/* endregion */

/* region [binding_base] */

struct k_webui_binding {

    void (*fn_unbind)(void *binding);

    int (*fn_js_set)(void *binding, const char *val);
};

static void *k__webui_binding_add(const char *label, size_t binding_struct_size) {

    void *binding = k_str_map_get(&k__webui.bindings, label);
    if (NULL != binding) {
        k_log_error("label `%s` already exists", label);
        return NULL;
    }

    binding = k_str_map_add(&k__webui.bindings, label, binding_struct_size);
    if (NULL == binding)
        return NULL;

    return binding;
}

static void k__webui_binding_remove(const char *label) {

    struct k_webui_binding *binding = k_str_map_get(&k__webui.bindings, label);
    if (NULL == binding)
        return;

    if (binding->fn_unbind != NULL) {
        binding->fn_unbind(binding);
    }

    k_str_map_remove(&k__webui.bindings, label);
}

static void k__webui_js_set_C_val(webui_event_t *e) {

    const char *label = webui_get_string_at(e, 0);

    struct k_webui_binding *binding = k_str_map_get(&k__webui.bindings, label);
    if (NULL == binding) {
        k_log_warn("binding not found");
        assert(0);
        return;
    }

    const char *val = webui_get_string_at(e, 1);

    k_log_trace("[set val] label: %s, val: %s", label, val);

    binding->fn_js_set(binding, val);
}

/* endregion */

/* region [bind_int] */

struct k_webui_int_binding {
    struct k_webui_binding base;

    int *p_int;

    enum k_webui_input_type input_type;

    int (*fn_webui_set)(int *p_int, int new_val);
};

static int k__webui_int_binding_js_set_val(void *binding_, const char *val_) {
    struct k_webui_int_binding *binding = binding_;

    int new_val;

    switch (binding->input_type) {
        case K_WEBUI_INT_RANGE:
            if (0 != parse_int(val_, &new_val))
                return -1;
            break;
        case K_WEBUI_CHECKBOX:
            new_val = (0 == strcmp(val_, "true")) ? 1 : 0;
            break;
        default:
            assert(0);
            return -1;
    }

    if (NULL == binding->fn_webui_set) {
        *binding->p_int = new_val;
        return 0;
    }

    return binding->fn_webui_set(binding->p_int, new_val);
}

int k_webui_bind_int(const char *label, int *p_int, const struct k_webui_int_options *options) {

    if (NULL == options) {
        static struct k_webui_int_options default_options = {
            .input_type = K_WEBUI_INT_RANGE,
            .range.min  = -100,
            .range.max  = 100,
            .range.step = 1,
        };
        options = &default_options;
    }

    struct k_webui_int_binding *binding = k__webui_binding_add(label, sizeof(struct k_webui_int_binding));
    if (NULL == binding)
        return -1;

    binding->base.fn_unbind = NULL;
    binding->base.fn_js_set = k__webui_int_binding_js_set_val;

    binding->p_int = p_int;
    binding->fn_webui_set = options->fn_webui_set;

    struct k_webui_widget_config widget;
    switch (options->input_type) {
        case K_WEBUI_INT_RANGE:
            widget.input_type = K_WEBUI_INT_RANGE;
            widget.int_range.min  = options->range.min;
            widget.int_range.max  = options->range.max;
            widget.int_range.step = options->range.step;
            break;
        case K_WEBUI_CHECKBOX:
            widget.input_type = K_WEBUI_CHECKBOX;
            break;
        default: {
            k_log_error("unsupported input type");
            return -1;
        }
    }
    binding->input_type = widget.input_type;

    k__webui_exec_js_add_widget(label, &widget);
    return 0;
}

/* endregion */

/* region [bind_float] */

struct k_webui_float_binding {
    struct k_webui_binding base;

    float *p_float;

    enum k_webui_input_type input_type;

    int (*fn_webui_set)(float *p_float, float new_val);
};

static int k__webui_float_binding_js_set_val(void *binding_, const char *val) {
    struct k_webui_float_binding *binding = binding_;

    float new_val;

    switch (binding->input_type) {
        case K_WEBUI_FLOAT_RANGE:
            if (0 != parse_float(val, &new_val))
                return -1;
            break;
        default:
            assert(0);
            return -1;
    }

    if (NULL == binding->fn_webui_set) {
        *binding->p_float = new_val;
        return 0;
    }

    return binding->fn_webui_set(binding->p_float, new_val);
}

int k_webui_bind_float(const char *label, float *p_float, const struct k_webui_float_options *options) {

    if (NULL == options) {
        static struct k_webui_float_options default_options = {
            .input_type = K_WEBUI_FLOAT_RANGE,
            .range.min  = 0.0f,
            .range.max  = 1.0f,
            .range.step = 0.01f,
        };
        options = &default_options;
    }

    struct k_webui_float_binding *binding = k__webui_binding_add(label, sizeof(struct k_webui_float_binding));
    if (NULL == binding)
        return -1;

    binding->base.fn_unbind = NULL;
    binding->base.fn_js_set = k__webui_float_binding_js_set_val;

    binding->p_float = p_float;
    binding->fn_webui_set = options->fn_webui_set;

    struct k_webui_widget_config widget;
    switch (options->input_type) {
        case K_WEBUI_FLOAT_RANGE: {
            widget.input_type = K_WEBUI_FLOAT_RANGE;
            widget.float_range.min  = options->range.min;
            widget.float_range.max  = options->range.max;
            widget.float_range.step = options->range.step;
            break;
        }
        default: {
            k_log_error("unsupported input type");
            return -1;
        }
    }
    binding->input_type = widget.input_type;

    k__webui_exec_js_add_widget(label, &widget);
    return 0;
}

/* endregion */

/* region [unbind] */

void k_webui_unbind(const char *label) {

}

/* endregion */
