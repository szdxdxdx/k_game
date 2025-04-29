#ifndef K_WEBUI_H
#define K_WEBUI_H

#include <stddef.h>

/* region [init] */

int k_webui_init(void);

void k_webui_close(void);

/* endregion */

/* region [log] */

void k_webui_log_info(const char *fmt, ...);

void k_webui_log_warn(const char *fmt, ...);

void k_webui_log_error(const char *fmt, ...);

/* endregion */

/* region [bind] */

struct k_webui_widget_config;

int k_webui_bind(const char *label, void *data, const struct k_webui_widget_config *widget);

void k_webui_unbind(const char *label);

enum k_webui_input_type {
    K_WEBUI_INT_RANGE,
    K_WEBUI_FLOAT_RANGE,
    K_WEBUI_CHECKBOX,
    K_WEBUI_BUTTON,
    K_WEBUI_INT_SELECT,
};

struct k_webui_int_range {
    int min;
    int max;
    int step;
    int (*on_input)(void *data, int val);
};

struct k_webui_float_range {
    float min;
    float max;
    float step;
    int (*on_input)(void *data, float val);
};

struct k_webui_checkbox {
    int (*on_change)(void *data, int val);
};

struct k_webui_button {
    void (*on_click)(void *data);
};

struct k_webui_int_select_option {
    int val;
    const char *text;
};

struct k_webui_int_select {
    struct k_webui_int_select_option *options;
    int count;
    int (*on_change)(void *data, int val);
};

struct k_webui_widget_config {
    enum k_webui_input_type input_type;
    union {
        struct k_webui_int_range   as_int_range;
        struct k_webui_float_range as_float_range;
        struct k_webui_checkbox    as_checkbox;
        struct k_webui_button      as_button;
        struct k_webui_int_select  as_int_select;
    };
};

/* endregion */

#endif
