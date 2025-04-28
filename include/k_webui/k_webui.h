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

void k_webui_unbind(const char *label);

/* endregion */

enum k_webui_bind_input_type {
    K_WEBUI_RANGE,
    K_WEBUI_CHECKBOX,
};

struct k_webui_bind_int_range {
    int min;
    int max;
    int step;
    void (*fn_set)(int *binding, int val);
};

struct k_webui_bind_int_checkbox {
    void (*fn_set)(int *binding, int val);
};

struct k_webui_bind_int_options {
    enum k_webui_bind_input_type input_type;
    union {
        struct k_webui_bind_int_range    range;
        struct k_webui_bind_int_checkbox checkbox;
    };
};

void k_webui_bind_int(const char *label, int *p_int, const struct k_webui_bind_int_options *options);

#endif
