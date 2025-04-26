#ifndef K_WEBUI_H
#define K_WEBUI_H

#include <stddef.h>

int k_webui_init(void);

void k_webui_close(void);

struct k_webui_bind_config {

    void *data;

    size_t data_size;

    int (*fn_init)(void *data, void *params);

    int (*fn_fini)(void *data);

    int (*fn_set)(void *data, const char *val);

    int (*fn_get)(void *data, const char *val);
};

int k_webui_puts(const char *s);

int k_webui_printf(const char *fmt, ...);

void k_webui_log_info(const char *fmt, ...);

void k_webui_log_warn(const char *fmt, ...);

void k_webui_log_error(const char *fmt, ...);

#endif
