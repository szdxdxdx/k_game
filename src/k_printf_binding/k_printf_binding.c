#include "k_printf_binding.h"


k_printf_callback_fn match_spec_xml(const char **str) {

    const char *p = *str;
    const char *s = *str;

    if ('k' == *p) p++; else return NULL;
    if ('_' == *p) p++; else return NULL;

    k_printf_callback_fn fn_callback;

    switch (*p) {

        case 'x': {
            p++;
            if ('m' == *p) p++; else return NULL;
            if ('l' == *p) p++; else return NULL;
            fn_callback = k__printf_spec_k_xml;
            goto done;
        }
        default:
            return NULL;
    }

done:
    *str += (p - s);
    return fn_callback;
}

static struct k_printf_config k__fmt = {
    .fn_match_spec = match_spec_xml
};

extern struct k_printf_config *k_fmt = &k__fmt;
