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

struct k_webui_bind_int_options;
struct k_webui_bind_float_options;

void k_webui_bind_int(const char *label, int *p_int, const struct k_webui_bind_int_options *options);

void k_webui_bind_float(const char *label, float *p_float, const struct k_webui_bind_float_options *options);

void k_webui_unbind(const char *label);

enum k_webui_bind_input_type {
    K_WEBUI_RANGE,
    K_WEBUI_CHECKBOX,
    K_WEBUI_BUTTON,
};

struct k_webui_bind_int_range {
    int min;
    int max;
    int step;
};

struct k_webui_bind_int_checkbox {
    void *_;
};

struct k_webui_bind_int_options {
    enum k_webui_bind_input_type input_type;
    union {
        struct k_webui_bind_int_range    range;
        struct k_webui_bind_int_checkbox checkbox;
    };
};

/* endregion */

#endif
