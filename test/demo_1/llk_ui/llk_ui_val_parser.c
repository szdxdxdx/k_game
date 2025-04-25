#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "./llk_ui_ext.h"

int llk__ui_parse_length_val(const char *str, float *get_val, enum llk_ui_unit *get_unit) {

    char *end;
    float val = strtof(str, &end);
    if (end == str)
        return -1;

    if (*end == '\0' || strncmp(end, "px", 2) == 0) {
        *get_val = val;
        *get_unit = LLK_UI_UNIT_PX;
        return 0;
    }
    else if (strncmp(end, "%", 1) == 0) {
        *get_val = val / 100.0f;
        *get_unit = LLK_UI_UNIT_PERCENT;
        return 0;
    }
    else if (strncmp(end, "vw", 2) == 0) {
        *get_val = val / 100.0f;
        *get_unit = LLK_UI_UNIT_VW;
        return 0;
    }
    else if (strncmp(end, "vh", 2) == 0) {
        *get_val = val / 100.0f;
        *get_unit = LLK_UI_UNIT_VH;
        return 0;
    }

    return -1;
}

int llk__ui_parse_color_val(const char *str, uint32_t *get_val, enum llk_ui_unit *get_unit) {

    if (*str == '\0') {
        *get_unit = LLK_UI_UNIT_RGBA;
        *get_val = 0x00000000;
        return 0;
    }

    if (*str != '#')
        return -1;

    const char *color_begin = str + 1;
    const char *p = color_begin;
    while (isxdigit(*p)) {
        p++;
        if (p - color_begin > 8)
            return -1;
    }

    if (*p != '\0')
        return -1;

    size_t color_len = p - color_begin;
    if (color_len != 6 && color_len != 8)
        return -1;

    uint32_t color_val = 0;
    p = color_begin;
    for (; '\0' != *p; p++) {
        char c = *p;
        uint8_t v;
        if ('0' <= c && c <= '9') {
            v = c - '0';
        } else if ('a' <= c && c <= 'f') {
            v = c - 'a' + 10;
        } else if ('A' <= c && c <= 'F') {
            v = c - 'A' + 10;
        } else {
            return -1;
        }

        color_val = (color_val << 4) | v;
    }

    if (color_len == 6) {
        color_val = (color_val << 8) | 0xFF;
    }

    *get_unit = LLK_UI_UNIT_RGBA;
    *get_val = color_val;
    return 0;
}
