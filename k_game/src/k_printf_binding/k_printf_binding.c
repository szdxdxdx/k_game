#include "k_printf_binding.h"

k_printf_spec_print_fn k_fmt(const char **str) {

    const char *p = *str;
    const char *s = *str;

    if ('k' == *p) p++; else return NULL;
    if ('_' == *p) p++; else return NULL;

    k_printf_spec_print_fn fn_callback;

    switch (*p) {

        case 'j': {
            p++;
            if ('s' == *p) p++; else return NULL;
            if ('o' == *p) p++; else return NULL;
            if ('n' == *p) p++; else return NULL;
            fn_callback = k__printf_k_json;
            goto done;
        }

        case 'x': {
            p++;
            if ('m' == *p) p++; else return NULL;
            if ('l' == *p) p++; else return NULL;
            fn_callback = k__printf_k_xml;
            goto done;
        }
        default:
            return NULL;
    }

done:
    *str += (p - s);
    return fn_callback;
}
