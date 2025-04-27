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

void k_webui_bind_int(const char *label, int *val);

void k_webui_unbind(const char *label);

struct k_webui_binding_config;

int k_webui_bind(const char *label, const struct k_webui_binding_config *config, void *params);

struct k_webui_binding_config {

    size_t data_size;

    int (*fn_init)(void *data, void *params);

    void (*fn_unbind)(void *data);

    int (*fn_webui_set)(void *data, const char *val);

    // TODO void (*fn_webui_get)(void *data, const char *req);
};

/* endregion */

#endif
