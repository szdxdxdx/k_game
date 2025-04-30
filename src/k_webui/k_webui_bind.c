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

/* region [init] */

static void k__webui_js_set_c_val(webui_event_t *e) {

    const char *label = webui_get_string_at(e, 0);
    struct k_webui_widget *widget = k__webui_bindings_map_get(label);
    if (NULL == widget)
        goto err;

    if (0 != widget->v_tbl->on_webui_set(widget, e))
        goto err;

    webui_return_int(e, 0);
    return;

err:
    webui_return_int(e, -1);
}

static void k__webui_js_get_c_val(webui_event_t *e) {

    const char *label = webui_get_string_at(e, 0);

    struct k_webui_widget *widget = k__webui_bindings_map_get(label);
    if (NULL == widget) {
        webui_return_string(e, "");
        return;
    }

    widget->v_tbl->on_webui_get(widget, e);
}

int k__webui_binding_init(void) {

    if (0 != k__webui_bindings_map_init())
        return -1;

    webui_bind(k__webui.window, "k__webui_set_c_val", k__webui_js_set_c_val);
    webui_bind(k__webui.window, "k__webui_get_c_val", k__webui_js_get_c_val);

    return 0;
}

void k__webui_binding_fini(void) {
    /* TODO */
}

/* endregion */

/* region [utils] */

static inline void *first_non_null(void *a, void *b) {
    return NULL != a ? a : b;
}

static inline int k__webui_get_int_at(webui_event_t *e, size_t idx) {
    long long int ll = webui_get_int_at(e, idx);
    return (ll < INT_MIN) ? INT_MIN : (ll > INT_MAX) ? INT_MAX : (int)ll;
}

static inline int k__webui_get_bool_att(webui_event_t *e, size_t idx) {
    bool b = webui_get_bool_at(e, idx);
    return b ? 1 : 0;
}

static inline float k__webui_get_float_at(webui_event_t *e, size_t idx) {
    return (float)webui_get_float_at(e, idx);
}

static inline const char *k__webui_get_str_at(webui_event_t *e, size_t idx) {
    return webui_get_string_at(e, idx);
}

/* endregion */

/* region [exec_js_add_ui_widget]

static void k__webui_exec_js_add_ui_widget(const char *label, const struct k_webui_widget_config *widget) {

    struct k_str_buf str_buf;
    char buf[1024];
    k_str_buf_init(&str_buf, NULL, sizeof(buf));

    k_str_buf_puts(&str_buf, "k__webui.bind({");
    {
        k_str_buf_k_printf(&str_buf, k__webui_fmt, "label:%'s,", label);

        switch (widget->input_type) {
            case K__WEBUI_INT_SLIDER:
                k_str_buf_puts(&str_buf, "type:'range',");
                k_str_buf_printf(&str_buf, "min:%d,",  widget->as_int_slider->min);
                k_str_buf_printf(&str_buf, "max:%d,",  widget->as_int_slider->max);
                k_str_buf_printf(&str_buf, "step:%d,", widget->as_int_slider->step);
                break;
            case K__WEBUI_FLOAT_SLIDER:
                k_str_buf_puts(&str_buf, "type:'range',");
                k_str_buf_printf(&str_buf, "min:%.3f,",  widget->as_float_slider->min);
                k_str_buf_printf(&str_buf, "max:%.3f,",  widget->as_float_slider->max);
                k_str_buf_printf(&str_buf, "step:%.3f,", widget->as_float_slider->step);
                break;
            case K__WEBUI_CHECKBOX:
                k_str_buf_puts(&str_buf, "type:'checkbox',");
                break;
            case K__WEBUI_BUTTON:
                k_str_buf_puts(&str_buf, "type:'button',");
                break;
            case K__WEBUI_INT_SELECT:
                k_str_buf_puts(&str_buf, "type:'select',");
                k_str_buf_puts(&str_buf, "options:[");
                {
                    size_t i = 0;
                    for (; i < widget->as_int_select->options_num; i++) {
                        k_str_buf_puts(&str_buf, "{");
                        {
                            struct k_webui_int_select_option *opt = &widget->as_int_select->options[i];
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

/* region [int_slider] */

struct k_webui_int_slider {

    struct k_webui_widget widget;

    void *data;

    int (*on_input)(void *data, int val);

    int (*on_read)(void *data);
};

void k__webui_int_slider_on_unbind(struct k_webui_widget *widget) {
}

int k__webui_int_slider_on_webui_set(struct k_webui_widget *widget, webui_event_t *e) {
    struct k_webui_int_slider *slider = (struct k_webui_int_slider *)widget;

    int val = k__webui_get_int_at(e, 1);

    if (NULL == slider->on_input) {
        *(int *)slider->data = val;
        return 0;
    }

    return slider->on_input(slider->data, val);
}

int k__webui_int_slider_on_webui_get(struct k_webui_widget *widget, webui_event_t *e) {

}

static struct k_webui_widget_v_tbl k__webui_int_slider_v_tbl = {
    .on_unbind    = k__webui_int_slider_on_unbind,
    .on_webui_set = k__webui_int_slider_on_webui_set,
    .on_webui_get = k__webui_int_slider_on_webui_get,
};

void k__webui_exec_js_add_int_slider(const char *label, const struct k_webui_int_slider_config *config) {

    struct k_str_buf str_buf;
    char buf[896];
    k_str_buf_init(&str_buf, buf, sizeof(buf));

    k_str_buf_puts(&str_buf, "k__webui.bind({");
    {
        k_str_buf_k_printf(&str_buf, k__webui_fmt, "label:%'s,", label);
        k_str_buf_puts(&str_buf, "type:'range',");
        k_str_buf_printf(&str_buf, "min:%d,",  config->min);
        k_str_buf_printf(&str_buf, "max:%d,",  config->max);
        k_str_buf_printf(&str_buf, "step:%d,", config->step);
    }
    k_str_buf_puts(&str_buf, "})");

    char *js = k_str_buf_get(&str_buf);
    k__webui_exec_js(js);

    k_str_buf_free(&str_buf);
}

int k_webui_bind_int_slider(const char *label, void *data, const struct k_webui_int_slider_config *config) {

    struct k_webui_int_slider *slider = NULL;

    slider = k__webui_malloc(sizeof(struct k_webui_int_slider));
    if (NULL == slider)
        return -1;

    if (0 != k__webui_widget_init(&slider->widget, label, &k__webui_int_slider_v_tbl))
        return -1;

    slider->data     = data;
    slider->on_input = config->on_input;
    slider->on_read  = config->on_read;

    k__webui_exec_js_add_int_slider(label, config);
    return 0;
}

/* endregion */

/* region [unbind] */

void k_webui_unbind(const char *label) {

}

/* endregion */
