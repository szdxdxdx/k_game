#include "k_printf.h"

#include "./k_webui_internal.h"

static void k__webui_print_str(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args) {
    (void)spec;

    char *c_str = va_arg(*args, char *);

    if (NULL == c_str) {
        k_printf_buf_puts_n(buf, "\"\"", 2);
        return;
    }

    char tmp_buf[512];

    char *p = tmp_buf;
    *p++ = '\"';

    while (*c_str != '\0') {
        switch (*c_str) {
            case '\"': case '\'':
            case '`' : case '$' :
            case '\\':
                *p++ = '\\';
                *p++ = *c_str++;
                break;
            case '\t':
                *p++ = '\\';
                *p++ = 't';
                c_str++;
                break;
            case '\r': case '\n':
                *p++ = '\\';
                *p++ = 'n';
                c_str++;
                break;
            default:
                *p++ = *c_str++;
                break;
        }

        /* 每轮循环最多往 `tmp_buf` 中写入 2 个字符，
         * 所以当 `tmp_buf` 快满（倒数第 2 位被写入）时，
         * 就要输出给 `k_printf` 了。
         *
         * 退出循环时，`tmp_buf` 一定不是全满的。
         */
        if (tmp_buf + sizeof(tmp_buf) - 2 <= p) {
            k_printf_buf_puts_n(buf, tmp_buf, p - tmp_buf);
            p = tmp_buf;
        }
    }

    *p++ = '\"';
    k_printf_buf_puts_n(buf, tmp_buf, p - tmp_buf);
}

k_printf_spec_print_fn k__webui_fmt(const char **str) {

    const char *ch = *str;
    if (ch[0] == '\'' && ch[1] == 's') {
        *str += 2;
        return k__webui_print_str;
    }

    return NULL;
}
