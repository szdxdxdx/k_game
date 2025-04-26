#ifndef K__WEBUI_K_PRINTF_H
#define K__WEBUI_K_PRINTF_H

#include <stddef.h>

int k__webui_sprintf(char *buf, const char *fmt, ...);

int k__webui_vsprintf(char *buf, const char *fmt, va_list args);

int k__webui_snprintf(char *buf, size_t n, const char *fmt, ...);

int k__webui_vsnprintf(char *buf, size_t n, const char *fmt, va_list args);

#endif
