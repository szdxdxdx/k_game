#include "k_printf_binding.h"
#include "../k_json/k_json_print.h"

void k__printf_k_json(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args) {
    k__json_print(buf, spec, args);
}
