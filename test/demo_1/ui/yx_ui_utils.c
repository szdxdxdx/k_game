
#include "./yx_ui_internal.h"

int yx__ui_parse_hex_color(const char *str, uint32_t *out_color) {
    if (!str || str[0] != '#' || strlen(str) != 9) return 0;

    uint32_t value = 0;
    int i = 1;
    for (; i < 9; ++i) {
        char c = str[i];
        if (!isxdigit(c))
            return 0;

        value <<= 4;
        if (c >= '0' && c <= '9')
            value |= (c - '0');
        else if (c >= 'a' && c <= 'f')
            value |= (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F')
            value |= (c - 'A' + 10);
    }

    *out_color = value;
    return 1;
}
