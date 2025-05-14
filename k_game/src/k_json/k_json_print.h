#ifndef K_JSON_PRINT_H
#define K_JSON_PRINT_H

#include "k_printf.h"

void k__json_print(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args);

#endif
