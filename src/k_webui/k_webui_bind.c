#include "k_str_buf.h"

#define K_LOG_TAG "k_webui:bind"
#include "k_log.h"

#include "./k_webui_internal.h"

/* region [utils] */

static inline void *first_non_null(void *a, void *b) {
    return NULL != a ? a : b;
}

/* endregion */

/* region [exec_js_add_ui_widget] */

static void k__webui_exec_js_add_ui_widget(const char *label, const struct k_webui_widget_config *widget) {

    struct k_str_buf str_buf;
    char buf[1024];
    k_str_buf_init(&str_buf, NULL, sizeof(buf));

    k_str_buf_puts(&str_buf, "k__webui.bind({");
    {
        k_str_buf_k_printf(&str_buf, k__webui_fmt, "label:%'s,", label);

        switch (widget->input_type) {
            case K_WEBUI_INT_SLIDER:
                k_str_buf_puts(&str_buf, "type:'range',");
                k_str_buf_printf(&str_buf, "min:%d,",  widget->as_int_slider.min);
                k_str_buf_printf(&str_buf, "max:%d,",  widget->as_int_slider.max);
                k_str_buf_printf(&str_buf, "step:%d,", widget->as_int_slider.step);
                break;
            case K_WEBUI_FLOAT_SLIDER:
                k_str_buf_puts(&str_buf, "type:'range',");
                k_str_buf_printf(&str_buf, "min:%.3f,",  widget->as_float_slider.min);
                k_str_buf_printf(&str_buf, "max:%.3f,",  widget->as_float_slider.max);
                k_str_buf_printf(&str_buf, "step:%.3f,", widget->as_float_slider.step);
                break;
            case K_WEBUI_CHECKBOX:
                k_str_buf_puts(&str_buf, "type:'checkbox',");
                break;
            case K_WEBUI_BUTTON:
                k_str_buf_puts(&str_buf, "type:'button',");
                break;
            case K_WEBUI_INT_SELECT:
                k_str_buf_puts(&str_buf, "type:'select',");
                k_str_buf_puts(&str_buf, "options:[");
                {
                    size_t i = 0;
                    for (; i < widget->as_int_select.count; i++) {
                        k_str_buf_puts(&str_buf, "{");
                        {
                            struct k_webui_int_select_option *opt = &widget->as_int_select.options[i];
                            k_str_buf_k_printf(&str_buf, k__webui_fmt, "text:%'s,", opt->text);
                            k_str_buf_printf(&str_buf, "val:%d,", opt->val);
                        }
                        k_str_buf_puts(&str_buf, "},");
                    }
                }
                k_str_buf_puts(&str_buf, "],");
                break;
            default:
                assert(0);
        }
    }
    k_str_buf_puts(&str_buf, "})");

    char *js = k_str_buf_get(&str_buf);
    k__webui_exec_js(js);

    k_str_buf_free(&str_buf);
}

/* endregion */

/* region [default_set/get_val] */

static int k__webui_set_int_default(void *data, int val) {
    *(int *)data = val;
    return 0;
}

static int k__webui_get_int_default(void *data) {
    return *(int *)data;
}

static int k__webui_set_float_default(void *data, float val) {
    *(float *)data = val;
    return 0;
}

static float k__webui_get_float_default(void *data) {
    return *(float *)data;
}

/* endregion */

/* region [bind] */

struct k_webui_binding {

    void *data;

    enum k_webui_input_type input_type;

    union {
        int  (*fn_set_i)(void *data, int val);
        int  (*fn_set_f)(void *data, float val);
        void (*on_click)(void *data);
    };

    union {
        int (*fn_get_i)(void *data);
        float (*fn_get_f)(void *data);
    };
};

static int k__webui_binding_init(struct k_webui_binding *binding, void *data, const struct k_webui_widget_config *widget) {

    binding->input_type = widget->input_type;
    binding->data = data;

    switch (widget->input_type) {
        case K_WEBUI_INT_SLIDER:
            binding->fn_set_i = first_non_null(widget->as_int_slider.on_input, k__webui_set_int_default);
            binding->fn_get_i = first_non_null(widget->as_int_slider.on_read,  k__webui_get_int_default);
            break;
        case K_WEBUI_FLOAT_SLIDER:
            binding->fn_set_f = first_non_null(widget->as_float_slider.on_input, k__webui_set_float_default);
            binding->fn_get_f = first_non_null(widget->as_float_slider.on_read,  k__webui_get_float_default);
            break;
        case K_WEBUI_CHECKBOX:
            binding->fn_set_i = first_non_null(widget->as_checkbox.on_change, k__webui_set_int_default);
            binding->fn_get_i = first_non_null(widget->as_checkbox.on_read,   k__webui_get_int_default);
            break;
        case K_WEBUI_BUTTON:
            if (NULL == widget->as_button.on_click) {
                k_log_error("button widget has no `on_click` callback");
                return -1;
            }
            binding->on_click = widget->as_button.on_click;
            binding->fn_get_i = NULL;
            break;
        case K_WEBUI_INT_SELECT:
            binding->fn_set_i = first_non_null(widget->as_int_select.on_change, k__webui_set_int_default);
            binding->fn_get_i = first_non_null(widget->as_int_select.on_read,   k__webui_get_int_default);
            break;
        default:
            assert(0);
            return -1;
    }

    return 0;
}

int k_webui_bind(const char *label, void *data, const struct k_webui_widget_config *widget) {

    if (NULL == label || '\0' == label[0])
        return -1;
    if (NULL == data)
        return -1;
    if (NULL == widget)
        return -1;

    if (NULL != k_str_map_get(&k__webui.bindings, label)) {
        k_log_error("label `%s` exists", label);
        goto err;
    }

    struct k_webui_binding *binding = k_str_map_add(&k__webui.bindings, label, sizeof(struct k_webui_binding));
    if (NULL == binding)
        goto err;

    if (0 != k__webui_binding_init(binding, data, widget)) {
        k_str_map_remove(&k__webui.bindings, label);
        goto err;
    }

    k__webui_exec_js_add_ui_widget(label, widget);
    return 0;

err:
    k_log_error("failed to add binding for label `%s`", label);
    return -1;
}

/* endregion */

/* region [js_set_c_val] */

static int k__webui_js_set_c_val_(webui_event_t *e) {

    const char *label = webui_get_string_at(e, 0);

    struct k_webui_binding *binding = k_str_map_get(&k__webui.bindings, label);
    if (NULL == binding) {
        assert(0);
        return -1;
    }

    switch (binding->input_type) {
        case K_WEBUI_INT_SLIDER: {
            long long int ll = webui_get_int_at(e, 1);
            int val = (ll < INT_MIN) ? INT_MIN :
                      (ll > INT_MAX) ? INT_MAX : (int)ll;

            binding->fn_set_i(binding->data, val);
            break;
        }
        case K_WEBUI_FLOAT_SLIDER: {
            double d = webui_get_float_at(e, 1);
            float val = (float)d;

            binding->fn_set_f(binding->data, val);
            break;
        }
        case K_WEBUI_CHECKBOX: {
            bool b = webui_get_bool_at(e, 1);
            int val = b ? 1 : 0;

            binding->fn_set_i(binding->data, val);
            break;
        }
        case K_WEBUI_BUTTON: {
            binding->on_click(binding->data);
            break;
        }
        case K_WEBUI_INT_SELECT: {
            long long int ll = webui_get_int_at(e, 1);
            int val = (ll < INT_MIN) ? INT_MIN :
                      (ll > INT_MAX) ? INT_MAX : (int)ll;

            binding->fn_set_i(binding->data, val);
            break;
        }
        default:
            assert(0);
            return -1;
    }

    return 0;
}

static void k__webui_js_set_c_val(webui_event_t *e) {
    k__webui_js_set_c_val_(e);
}

/* endregion */

/* region [js_get_c_val] */

static int k__webui_js_get_c_val_(webui_event_t *e) {

    const char *label = webui_get_string_at(e, 0);

    struct k_webui_binding *binding = k_str_map_get(&k__webui.bindings, label);
    if (NULL == binding) {
        assert(0);
        return -1;
    }

    switch (binding->input_type) {
        case K_WEBUI_INT_SLIDER: {
            int val = binding->fn_get_i(binding->data);
            webui_return_int(e, val);
            break;
        }
        case K_WEBUI_FLOAT_SLIDER: {
            float val = binding->fn_get_f(binding->data);
            webui_return_float(e, val);
            break;
        }
        case K_WEBUI_CHECKBOX: {
            bool val = binding->fn_get_i(binding->data);
            webui_return_bool(e, val);
            break;
        }
        case K_WEBUI_BUTTON: {
            assert(0);
            return -1;
        }
        case K_WEBUI_INT_SELECT: {
            int val = binding->fn_get_i(binding->data);
            webui_return_int(e, val);
            break;
        }
        default:
            assert(0);
            return -1;
    }

    return 0;
}

static void k__webui_js_get_c_val(webui_event_t *e) {
    k__webui_js_get_c_val_(e);
}

/* endregion */

/* region [unbind] */

void k_webui_unbind(const char *label) {

}

/* endregion */

/* region [init] */

int k__webui_bindings_map_init(void) {

    struct k_str_map_options options;
    options.fn_malloc = k__webui_mem_alloc;
    options.fn_free   = k__webui_mem_free;
    if (NULL == k_str_map_construct(&k__webui.bindings, &options))
        return -1;

    webui_bind(k__webui.window, "k__webui_set_c_val", k__webui_js_set_c_val);
    webui_bind(k__webui.window, "k__webui_get_c_val", k__webui_js_get_c_val);

    return 0;
}

void k__webui_bindings_map_clear(void) {
    /* TODO */
}

/* endregion */
