#include "k_str_buf.h"

#define K_LOG_TAG "k_webui:bind"
#include "k_log.h"

#include "./k_webui_internal.h"

struct k_webui_widget;
struct k_webui_widget_v_tbl;

/* region [bindings_map] */

static struct k_str_map bindings_map;

static int k__webui_bindings_map_init(void) {
    struct k_str_map_options options;
    options.fn_malloc = k__webui_malloc;
    options.fn_free   = k__webui_free;
    if (NULL == k_str_map_construct(&bindings_map, &options))
        return -1;

    return 0;
}

static void k__webui_bindings_map_clear(void) {
    /* TODO */
}

static int k__webui_bindings_map_add(const char *label, struct k_webui_widget *widget) {
    assert(NULL != label && '\0' != label[0]);

    struct k_webui_widget **w = k_str_map_add(&bindings_map, label, sizeof(struct k_webui_widget *));
    if (NULL == widget) {
        return -1;
    }

    *w = widget;
    return 0;
}

static void k__webui_bindings_map_remove(const char *label) {
    k_str_map_remove(&bindings_map, label);
}

static struct k_webui_widget *k__webui_bindings_map_get(const char *label) {
    struct k_webui_widget **w = k_str_map_get(&bindings_map, label);
    return w ? *w : NULL;
}

/* endregion */

/* region [widget] */

struct k_webui_widget {
    struct k_webui_widget_v_tbl *v_tbl;
};

struct k_webui_widget_v_tbl {
    void (*on_unbind)(struct k_webui_widget *widget);
    int (*on_webui_set)(struct k_webui_widget *widget, webui_event_t *e);
    int (*on_webui_get)(struct k_webui_widget *widget, webui_event_t *e);
};

int k__webui_widget_init(struct k_webui_widget *widget, const char *label, struct k_webui_widget_v_tbl *v_tbl) {
    widget->v_tbl = v_tbl;
    return k__webui_bindings_map_add(label, widget);
}

/* endregion */

/* region [js_call_c] */

static void k__webui_js_set_c_val(webui_event_t *e) {
    const char *label = webui_get_string_at(e, 0);

    struct k_webui_widget *widget = k__webui_bindings_map_get(label);
    if (NULL == widget) {
        webui_return_int(e, -1);
        return;
    }

    if (0 != widget->v_tbl->on_webui_set(widget, e)) {
        webui_return_int(e, -2);
        return;
    }

    webui_return_int(e, 0);
}

static void k__webui_js_get_c_val(webui_event_t *e) {
    const char *label = webui_get_string_at(e, 0);

    struct k_webui_widget *widget = k__webui_bindings_map_get(label);
    if (NULL == widget)
        return;

    widget->v_tbl->on_webui_get(widget, e);
}

static void k__webui_js_unbind(webui_event_t *e) {
    const char *label = webui_get_string_at(e, 0);

    struct k_webui_widget *widget = k__webui_bindings_map_get(label);
    if (NULL == widget)
        return;

    k__webui_bindings_map_remove(label);
    widget->v_tbl->on_unbind(widget);
}

/* endregion */

/* region [init] */

int k__webui_binding_init(void) {

    if (0 != k__webui_bindings_map_init())
        return -1;

    webui_bind(k__webui.window, "k__webui_set_c_val", k__webui_js_set_c_val);
    webui_bind(k__webui.window, "k__webui_get_c_val", k__webui_js_get_c_val);
    webui_bind(k__webui.window, "k__webui_unbind", k__webui_js_unbind);
    return 0;
}

void k__webui_binding_fini(void) {
    /* TODO */
}

/* endregion */

/* region [get_js_fn_arg] */

static inline int k__webui_get_int_at(webui_event_t *e, size_t idx) {
    long long int ll = webui_get_int_at(e, idx);
    return (ll < INT_MIN) ? INT_MIN : (ll > INT_MAX) ? INT_MAX : (int)ll;
}

static inline int k__webui_get_bool_at(webui_event_t *e, size_t idx) {
    return webui_get_bool_at(e, idx) ? 1 : 0;
}

static inline float k__webui_get_float_at(webui_event_t *e, size_t idx) {
    return (float)webui_get_float_at(e, idx);
}

static inline const char *k__webui_get_str_at(webui_event_t *e, size_t idx) {
    return webui_get_string_at(e, idx);
}

/* endregion */

/* region [text] */

struct k_webui_text {
    struct k_webui_widget widget;

    void *data;

    int (*on_read)(void *data, struct k_str_buf *buf);
};

static void k__webui_text_on_unbind(struct k_webui_widget *widget) {
    struct k_webui_text *text = (struct k_webui_text *)widget;
    k__webui_free(text);
}

static int k__webui_text_on_get(struct k_webui_widget *widget, webui_event_t *e) {
    struct k_webui_text *text = (struct k_webui_text *)widget;

    struct k_str_buf buf;
    char default_buf[896];
    k_str_buf_init(&buf, default_buf, sizeof(default_buf));

    if (0 != text->on_read(text->data, &buf)) {
        return -1;
    }

    const char *s = k_str_buf_get(&buf);
    webui_return_string(e, s);

    k_str_buf_free(&buf);
    return 0;
}

void k__webui_exec_js_add_text(const char *label, const struct k_webui_text_config *config) {

    struct k_str_buf str_buf;
    char buf[896];
    k_str_buf_init(&str_buf, buf, sizeof(buf));

    k_str_buf_puts(&str_buf, "k__webui.bind({");
    {
        k_str_buf_k_printf(&str_buf, k__webui_fmt, "label:%'s,", label);
        k_str_buf_puts(&str_buf, "type:'text',");
    }
    k_str_buf_puts(&str_buf, "})");

    char *js = k_str_buf_get(&str_buf);
    k__webui_exec_js(js);

    k_str_buf_free(&str_buf);
}

int k_webui_bind_text(const char *label, void *data, const struct k_webui_text_config *config) {

    if (NULL == label || '\0' == label[0])
        return -1;
    if (NULL == config)
        return -1;

    struct k_webui_text *text = k__webui_malloc(sizeof(struct k_webui_text));
    if (NULL == text)
        return -1;

    if (0 != k__webui_bindings_map_add(label, &text->widget)) {
        k__webui_free(text);
        return -1;
    }

    static struct k_webui_widget_v_tbl v_tbl = {
        .on_unbind    = k__webui_text_on_unbind,
        .on_webui_set = NULL,
        .on_webui_get = k__webui_text_on_get,
    };
    text->widget.v_tbl = &v_tbl;

    text->data = data;
    text->on_read = config->on_read;

    k__webui_exec_js_add_text(label, config);
    return 0;
}

/* endregion */

/* region [slider] */

struct k_webui_slider {
    struct k_webui_widget widget;

    void *data;

    enum k_webui_slider_type type;
    union {
        int (*on_input_i)(void *data, int val);
        int (*on_input_f)(void *data, float val);
    };
    union {
        int (*on_read_i)(void *data, int *result);
        int (*on_read_f)(void *data, float *result);
    };
};

static void k__webui_slider_on_unbind(struct k_webui_widget *widget) {
    struct k_webui_slider *slider = (struct k_webui_slider *)widget;
    k__webui_free(slider);
}

static int k__webui_slider_on_set(struct k_webui_widget *widget, webui_event_t *e) {
    struct k_webui_slider *slider = (struct k_webui_slider *)widget;

    switch (slider->type) {
        case K_WEBUI_INT_SLIDER: {
            int val = k__webui_get_int_at(e, 1);
            if (NULL == slider->on_input_i) {
                *(int *)slider->data = val;
                return 0;
            } else {
                return slider->on_input_i(slider->data, val);
            }
        }
        case K_WEBUI_FLOAT_SLIDER: {
            float val = k__webui_get_float_at(e, 1);
            if (NULL == slider->on_input_f) {
                *(float *)slider->data = val;
                return 0;
            } else {
                return slider->on_input_f(slider->data, val);
            }
        }
        default:
            assert(0);
            return -1;
    }
}

static int k__webui_slider_on_get(struct k_webui_widget *widget, webui_event_t *e) {
    struct k_webui_slider *slider = (struct k_webui_slider *)widget;

    switch (slider->type) {
        case K_WEBUI_INT_SLIDER: {
            int result;
            if (NULL == slider->on_read_i) {
                result = *(int *)slider->data;
            } else {
                if (0 != slider->on_read_i(slider->data, &result))
                    return -1;
            }
            webui_return_int(e, result);
            return 0;
        }
        case K_WEBUI_FLOAT_SLIDER: {
            float result;
            if (NULL == slider->on_read_f) {
                result = *(float *)slider->data;
            } else {
                if (0 != slider->on_read_f(slider->data, &result))
                    return -1;
            }
            webui_return_float(e, result);
            return 0;
        }
        default:
            assert(0);
            return -1;
    }
}

struct k_webui_slider_config {
    enum k_webui_slider_type slider_type;
    union {
        const struct k_webui_int_slider_config   *int_slider;
        const struct k_webui_float_slider_config *float_slider;
    };
};

void k__webui_exec_js_add_slider(const char *label, const struct k_webui_slider_config *config) {

    struct k_str_buf str_buf;
    char buf[896];
    k_str_buf_init(&str_buf, buf, sizeof(buf));

    k_str_buf_puts(&str_buf, "k__webui.bind({");
    {
        k_str_buf_k_printf(&str_buf, k__webui_fmt, "label:%'s,", label);
        k_str_buf_puts(&str_buf, "type:'range',");
        switch (config->slider_type) {
            case K_WEBUI_INT_SLIDER:
                k_str_buf_printf(&str_buf, "min:%d,",  config->int_slider->min);
                k_str_buf_printf(&str_buf, "max:%d,",  config->int_slider->max);
                k_str_buf_printf(&str_buf, "step:%d,", config->int_slider->step);
                break;
            case K_WEBUI_FLOAT_SLIDER:
                k_str_buf_printf(&str_buf, "min:%f,",  config->float_slider->min);
                k_str_buf_printf(&str_buf, "max:%f,",  config->float_slider->max);
                k_str_buf_printf(&str_buf, "step:%f,", config->float_slider->step);
                break;
        }
    }
    k_str_buf_puts(&str_buf, "})");

    char *js = k_str_buf_get(&str_buf);
    k__webui_exec_js(js);

    k_str_buf_free(&str_buf);
}

static int k__webui_bind_slider(const char *label, void *data, const struct k_webui_slider_config *config) {

    if (NULL == label || '\0' == label[0])
        return -1;
    if (NULL == config)
        return -1;

    struct k_webui_slider *slider = k__webui_malloc(sizeof(struct k_webui_slider));
    if (NULL == slider)
        return -1;

    if (0 != k__webui_bindings_map_add(label, &slider->widget)) {
        k__webui_free(slider);
        return -1;
    }

    static struct k_webui_widget_v_tbl v_tbl = {
        .on_unbind    = k__webui_slider_on_unbind,
        .on_webui_set = k__webui_slider_on_set,
        .on_webui_get = k__webui_slider_on_get,
    };
    slider->widget.v_tbl = &v_tbl;

    slider->data = data;

    slider->type = config->slider_type;
    switch (config->slider_type) {
        case K_WEBUI_INT_SLIDER:
            slider->on_input_i = config->int_slider->on_input;
            slider->on_read_i  = config->int_slider->on_read;
            break;
        case K_WEBUI_FLOAT_SLIDER:
            slider->on_input_f = config->float_slider->on_input;
            slider->on_read_f  = config->float_slider->on_read;
            break;
        default:
            assert(0);
    }

    k__webui_exec_js_add_slider(label, config);
    return 0;
}

int k_webui_bind_int_slider(const char *label, void *data, const struct k_webui_int_slider_config *config) {
    struct k_webui_slider_config config_;
    config_.slider_type = K_WEBUI_INT_SLIDER;
    config_.int_slider  = config;
    return k__webui_bind_slider(label, data, &config_);
}

int k_webui_bind_float_slider(const char *label, void *data, const struct k_webui_float_slider_config *config) {
    struct k_webui_slider_config config_;
    config_.slider_type  = K_WEBUI_FLOAT_SLIDER;
    config_.float_slider = config;
    return k__webui_bind_slider(label, data, &config_);
}

/* endregion */

/* region [checkbox] */

struct k_webui_checkbox {
    struct k_webui_widget widget;

    void *data;
    int (*on_change)(void *data, int checked);
    int (*on_read)(void *data, int *result);
};

static void k__webui_checkbox_on_unbind(struct k_webui_widget *widget) {
    struct k_webui_checkbox *checkbox = (struct k_webui_checkbox *)widget;
    k__webui_free(checkbox);
}

static int k__webui_checkbox_on_set(struct k_webui_widget *widget, webui_event_t *e) {
    struct k_webui_checkbox *checkbox = (struct k_webui_checkbox *)widget;

    int checked = k__webui_get_bool_at(e, 1);
    if (NULL == checkbox->on_change) {
        *(int *)checkbox->data = checked;
        return 0;
    } else {
        return checkbox->on_change(checkbox->data, checked);
    }
}

static int k__webui_checkbox_on_get(struct k_webui_widget *widget, webui_event_t *e) {
    struct k_webui_checkbox *checkbox = (struct k_webui_checkbox *)widget;

    int checked;
    if (NULL == checkbox->on_read) {
        checked = *(int *)checkbox->data;
    } else {
        if (0 != checkbox->on_read(checkbox->data, &checked))
            return -1;
    }

    int result = checked ? 1 : 0;
    webui_return_int(e, result);
    return 0;
}

void k__webui_exec_js_add_checkbox(const char *label, const struct k_webui_checkbox_config *config) {

    struct k_str_buf str_buf;
    char buf[896];
    k_str_buf_init(&str_buf, buf, sizeof(buf));

    k_str_buf_puts(&str_buf, "k__webui.bind({");
    {
        k_str_buf_k_printf(&str_buf, k__webui_fmt, "label:%'s,", label);
        k_str_buf_puts(&str_buf, "type:'checkbox',");
    }
    k_str_buf_puts(&str_buf, "})");

    char *js = k_str_buf_get(&str_buf);
    k__webui_exec_js(js);

    k_str_buf_free(&str_buf);
}

int k_webui_bind_checkbox(const char *label, void *data, const struct k_webui_checkbox_config *config) {

    if (NULL == label || '\0' == label[0])
        return -1;
    if (NULL == config)
        return -1;

    struct k_webui_checkbox *checkbox = k__webui_malloc(sizeof(struct k_webui_checkbox));
    if (NULL == checkbox)
        return -1;

    if (0 != k__webui_bindings_map_add(label, &checkbox->widget)) {
        k__webui_free(checkbox);
        return -1;
    }

    static struct k_webui_widget_v_tbl v_tbl = {
        .on_unbind    = k__webui_checkbox_on_unbind,
        .on_webui_set = k__webui_checkbox_on_set,
        .on_webui_get = k__webui_checkbox_on_get,
    };
    checkbox->widget.v_tbl = &v_tbl;

    checkbox->data = data;
    checkbox->on_change = config->on_change;
    checkbox->on_read   = config->on_read;

    k__webui_exec_js_add_checkbox(label, config);
    return 0;
}

/* endregion */

/* region [select] */

struct k_webui_int_select {
    struct k_webui_widget widget;

    void *data;
    int (*on_change)(void *data, int val);
    int (*on_read)(void *data, int *result);
};

static void k__webui_int_select_on_unbind(struct k_webui_widget *widget) {
    struct k_webui_int_select *select = (struct k_webui_int_select *)widget;
    k__webui_free(select);
}

static int k__webui_int_select_on_set(struct k_webui_widget *widget, webui_event_t *e) {
    struct k_webui_int_select *select = (struct k_webui_int_select *)widget;

    int val = k__webui_get_int_at(e, 1);
    if (NULL == select->on_change) {
        *(int *)select->data = val;
        return 0;
    } else {
        return select->on_change(select->data, val);
    }
}

static int k__webui_int_select_on_get(struct k_webui_widget *widget, webui_event_t *e) {
    struct k_webui_int_select *select = (struct k_webui_int_select *)widget;

    int result;
    if (NULL == select->on_read) {
        result = *(int *)select->data;
    } else {
        if (0 != select->on_read(select->data, &result))
            return -1;
    }

    webui_return_int(e, result);
    return 0;
}

void k__webui_exec_js_add_int_select(const char *label, const struct k_webui_int_select_config *config) {

    struct k_str_buf str_buf;
    char buf[896];
    k_str_buf_init(&str_buf, buf, sizeof(buf));

    k_str_buf_puts(&str_buf, "k__webui.bind({");
    {
        k_str_buf_k_printf(&str_buf, k__webui_fmt, "label:%'s,", label);
        k_str_buf_puts(&str_buf, "type:'select',");
        {
            k_str_buf_puts(&str_buf, "options:[");
            {
                size_t i = 0;
                for (; i < config->options_num; i++) {
                    k_str_buf_puts(&str_buf, "{");
                    {
                        struct k_webui_int_select_option *opt = &config->options[i];
                        k_str_buf_k_printf(&str_buf, k__webui_fmt, "text:%'s,", opt->text);
                        k_str_buf_printf(&str_buf, "val:%d,", opt->val);
                    }
                    k_str_buf_puts(&str_buf, "},");
                }
            }
            k_str_buf_puts(&str_buf, "],");
        }
    }
    k_str_buf_puts(&str_buf, "})");

    char *js = k_str_buf_get(&str_buf);
    k__webui_exec_js(js);

    k_str_buf_free(&str_buf);
}

int k_webui_bind_int_select(const char *label, void *data, const struct k_webui_int_select_config *config) {

    if (NULL == label || '\0' == label[0])
        return -1;
    if (NULL == config)
        return -1;

    struct k_webui_int_select *select = k__webui_malloc(sizeof(struct k_webui_int_select));
    if (NULL == select)
        return -1;

    if (0 != k__webui_bindings_map_add(label, &select->widget)) {
        k__webui_free(select);
        return -1;
    }

    static struct k_webui_widget_v_tbl v_tbl = {
        .on_unbind    = k__webui_int_select_on_unbind,
        .on_webui_set = k__webui_int_select_on_set,
        .on_webui_get = k__webui_int_select_on_get,
    };
    select->widget.v_tbl = &v_tbl;

    select->data = data;
    select->on_change = config->on_change;
    select->on_read   = config->on_read;

    k__webui_exec_js_add_int_select(label, config);
    return 0;
}

/* endregion */

/* region [button] */

struct k_webui_button {
    struct k_webui_widget widget;

    void *data;
    void (*on_click)(void *data);
};

static void k__webui_button_on_unbind(struct k_webui_widget *widget) {
    struct k_webui_button *button = (struct k_webui_button *)widget;
    k__webui_free(button);
}

static int k__webui_button_on_set(struct k_webui_widget *widget, webui_event_t *e) {
    (void)e;
    struct k_webui_button *button = (struct k_webui_button *)widget;

    button->on_click(button->data);
    return 0;
}

void k__webui_exec_js_add_button(const char *label, const struct k_webui_button_config *config) {

    struct k_str_buf str_buf;
    char buf[896];
    k_str_buf_init(&str_buf, buf, sizeof(buf));

    k_str_buf_puts(&str_buf, "k__webui.bind({");
    {
        k_str_buf_k_printf(&str_buf, k__webui_fmt, "label:%'s,", label);
        k_str_buf_puts(&str_buf, "type:'button',");
    }
    k_str_buf_puts(&str_buf, "})");

    char *js = k_str_buf_get(&str_buf);
    k__webui_exec_js(js);

    k_str_buf_free(&str_buf);
}

int k_webui_bind_button(const char *label, void *data, const struct k_webui_button_config *config) {

    if (NULL == label || '\0' == label[0])
        return -1;
    if (NULL == config)
        return -1;
    if (NULL == config->on_click)
        return -1;

    struct k_webui_button *button = k__webui_malloc(sizeof(struct k_webui_button));
    if (NULL == button)
        return -1;

    if (0 != k__webui_bindings_map_add(label, &button->widget)) {
        k__webui_free(button);
        return -1;
    }

    static struct k_webui_widget_v_tbl v_tbl = {
        .on_unbind    = k__webui_button_on_unbind,
        .on_webui_set = k__webui_button_on_set,
        .on_webui_get = NULL,
    };
    button->widget.v_tbl = &v_tbl;

    button->data = data;
    button->on_click = config->on_click;

    k__webui_exec_js_add_button(label, config);
    return 0;
}

/* endregion */

/* region [unbind] */

void k_webui_unbind(const char *label) {

    struct k_webui_widget *widget = k__webui_bindings_map_get(label);
    if (NULL == widget)
        return;

    k__webui_bindings_map_remove(label);
    widget->v_tbl->on_unbind(widget);

    k__webui_exec_js_fmt("k__webui.unbind_by_label(%'s)", label);
}

/* endregion */
