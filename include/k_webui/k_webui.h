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

enum k_webui_input_type {
    K_WEBUI_INT_RANGE,
    K_WEBUI_FLOAT_RANGE,
    K_WEBUI_CHECKBOX,
    K_WEBUI_BUTTON,
};

struct k_webui_int_range {
    int min;
    int max;
    int step;
};

struct k_webui_float_range {
    float min;
    float max;
    float step;
};

struct k_webui_checkbox {
    void *_;
};

struct k_webui_button {
    void *_;
};

struct k_webui_int_options {
    enum k_webui_input_type input_type;
    union {
        struct k_webui_int_range range;
        struct k_webui_checkbox  checkbox;
    };
};

struct k_webui_float_options {
    enum k_webui_input_type input_type;
    union {
        struct k_webui_float_range range;
    };
};

int k_webui_bind_int(const char *label, int *p_int, const struct k_webui_int_options *options);

int k_webui_bind_float(const char *label, float *p_float, const struct k_webui_float_options *options);

void k_webui_unbind(const char *label);

/* endregion */

#endif
