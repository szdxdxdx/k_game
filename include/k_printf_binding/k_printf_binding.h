#ifndef K_PRINTF_BINDING_H
#define K_PRINTF_BINDING_H

#include <stdarg.h>

#include "k_printf.h"

extern struct k_printf_config *k_fmt;

/**
 * \brief 打印 json
 *
 * 格式说明符：`%k_json` - `struct k_json *`
 */
void k__printf_spec_k_json(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args);

/**
 * \brief 打印 xml 节点
 *
 * 格式说明符：`%k_xml` - `struct k_xml_node *`
 */
void k__printf_spec_k_xml(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args);

#endif
