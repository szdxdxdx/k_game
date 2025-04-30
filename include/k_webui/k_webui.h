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

/* region [slider] */

enum k_webui_slider_type {
    K_WEBUI_INT_SLIDER,
    K_WEBUI_FLOAT_SLIDER,
};

struct k_webui_int_slider_config {
    int min;
    int max;
    int step;
    int (*on_input)(void *data, int val);
    int (*on_read)(void *data, int *result);
};

#define K_WEBUI_INT_SLIDER_INIT \
{ \
    .min      = 0,    \
    .max      = 100,  \
    .step     = 1,    \
    .on_input = NULL, \
    .on_read  = NULL, \
}

int k_webui_bind_int_slider(const char *label, void *data, const struct k_webui_int_slider_config *config);

/* endregion */

/* region [float_slider] */

struct k_webui_float_slider_config {
    float min;
    float max;
    float step;
    int (*on_input)(void *data, float val);
    int (*on_read)(void *data, float *result);
};

#define K_WEBUI_FLOAT_SLIDER_INIT \
{ \
    .min      = 0.0f,   \
    .max      = 100.0f, \
    .step     = 0.01f,  \
    .on_input = NULL,   \
    .on_read  = NULL,   \
}

int k_webui_bind_float_slider(const char *label, void *data, const struct k_webui_float_slider_config *config);

/* endregion */

/* region [checkbox] */

struct k_webui_checkbox_config {
    int (*on_change)(void *data, int checked);
    int (*on_read)(void *data, int *result);
};

#define K_WEBUI_CHECKBOX_INIT \
{ \
    .on_change = NULL, \
    .on_read   = NULL, \
}

int k_webui_bind_checkbox(const char *label, void *data, const struct k_webui_checkbox_config *config);

/* endregion */

/* region [button] */

struct k_webui_button_config {
    void (*on_click)(void *data);
};

#define K_WEBUI_BUTTON_INIT \
{ \
    .on_click = NULL, \
}

int k_webui_bind_button(const char *label, void *data, const struct k_webui_button_config *config);

/* endregion */

/* region [int_select] */

struct k_webui_int_select_option {
    int val;
    const char *text;
};

struct k_webui_int_select_config {
    struct k_webui_int_select_option *options;
    size_t options_num;
    int (*on_change)(void *data, int val);
    int (*on_read)(void *data);
};

#define K_WEBUI_INT_SELECT_CONFIG_INIT \
{ \
    .options     = NULL, \
    .options_num = 0,    \
    .on_change   = NULL, \
    .on_read     = NULL, \
}

int k_webui_bind_int_select(const char *label, void *data, const struct k_webui_int_select_config *config);

/* endregion */

void k_webui_unbind(const char *label);

/* endregion */

#endif
