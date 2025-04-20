#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "k_read_file.h"
#include "k_xml.h"
#include "k_log.h"

#include "./llk_ui_build.h"
#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"

static int parse_length_val(const char *str, float *get_val, enum llk_ui_unit *get_unit) {

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

    return -1;
}

static int parse_color_val(const char *str, uint32_t *get_val) {

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

    *get_val = color_val;
    return 0;
}

void llk__ui_build_elem_attr_from_xml(struct llk_ui_elem *elem, const char *attr_key, const char *attr_val) {

    if (0 == strncmp(attr_key, "w", 1)) {
        float val;
        enum llk_ui_unit unit;
        if (0 == parse_length_val(attr_val, &val, &unit)) {
            llk_ui_float_set(&elem->w, val, unit);
            return;
        } else {
            k_log_warn("Illegal attribute value when building llk UI element, key: `%s`, value: `%s`", attr_key, attr_val);
        }
    }
    else if (0 == strncmp(attr_key, "h", 1)) {
        float val;
        enum llk_ui_unit unit;
        if (0 == parse_length_val(attr_val, &val, &unit)) {
            llk_ui_float_set(&elem->h, val, unit);
            return;
        }
    }
    else if (0 == strncmp(attr_key, "left", 4)) {
        float val;
        enum llk_ui_unit unit;
        if (0 == parse_length_val(attr_val, &val, &unit)) {
            llk_ui_float_set(&elem->left, val, unit);
            return;
        }
    }
    else if (0 == strncmp(attr_key, "right", 5)) {
        float val;
        enum llk_ui_unit unit;
        if (0 == parse_length_val(attr_val, &val, &unit)) {
            llk_ui_float_set(&elem->right, val, unit);
            return;
        }
    }
    else if (0 == strncmp(attr_key, "top", 3)) {
        float val;
        enum llk_ui_unit unit;
        if (0 == parse_length_val(attr_val, &val, &unit)) {
            llk_ui_float_set(&elem->top, val, unit);
            return;
        }
    }
    else if (0 == strncmp(attr_key, "bottom", 6)) {
        float val;
        enum llk_ui_unit unit;
        if (0 == parse_length_val(attr_val, &val, &unit)) {
            llk_ui_float_set(&elem->bottom, val, unit);
            return;
        }
    }
    else if (0 == strncmp(attr_key, "background-color", 16)) {
        uint32_t color;
        if (0 == parse_color_val(attr_val, &color)) {
            elem->background_color = color;
            return;
        }
    }

    k_log_warn("Unused attribute when building llk UI element, key: `%s`, value: `%s`", attr_key, attr_val);
}

struct llk_ui_elem *llk__ui_build_elem_from_xml(struct llk_ui_elem *parent, struct k_xml_node *xml) {

    struct llk_ui_elem *elem = llk_ui_create_elem(parent->ui);
    if (NULL == elem)
        return NULL;

    struct k_xml_node *xml_child;
    for (k_xml_for_each_child(xml, xml_child)) {
        if (K_XML_ELEM_NODE == k_xml_get_type(xml_child)) {

            if (NULL == llk__ui_build_elem_from_xml(elem, xml_child))
                goto err;
        }
    }

    struct k_xml_attr *attr;
    const char *key;
    const char *val;
    for (k_xml_for_each_attr(xml, attr, key, val)) {
        llk__ui_build_elem_attr_from_xml(elem, key, val);
    }

    llk_ui_append_child(parent, elem);

    return elem;

err:
    k_log_error("Failed to build llk UI element from xml node, node tag: `%s`", k_xml_get_tag(xml));
    return NULL;
}

struct llk_ui_context *llk__ui_build_from_xml(struct k_xml_node *xml) {

    struct llk_ui_context *ui = llk_ui_create_context();
    if (NULL == ui)
        return NULL;

    struct llk_ui_elem *elem = llk__ui_build_elem_from_xml(ui->root, xml);
    if (NULL == elem) {
        llk_ui_destroy_context(ui);
        k_log_error("Failed to build llk UI from xml");
        return NULL;
    }

    return ui;
}

struct llk_ui_context *llk_ui_build_from_xml_file(const char *file_path) {

    char *text = k_read_txt_file(file_path, NULL, 0, NULL);
    if (NULL == text) {
        k_log_error("Failed to read file `%s`", file_path);
        return NULL;
    }

    struct k_xml_node *xml = k_xml_parse(text);

    if (NULL == xml) {
        free(text);
        k_log_error("Failed to parse xml");
        return NULL;
    }

    struct llk_ui_context *ui = llk__ui_build_from_xml(xml);

    k_xml_free(xml);
    free(text);

    return ui;
}
