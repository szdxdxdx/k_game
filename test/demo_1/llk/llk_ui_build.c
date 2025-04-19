#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

    if (*end == '\0') {
        *get_val = val;
        *get_unit = LLK_UI_UNIT_PX;
        return 0;
    }

    if (strncmp(end, "px", 2) == 0) {
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

int llk__ui_build_elem_attr_from_xml(struct llk_ui_elem *elem, const char *attr_key, const char *attr_val) {

    if (0 == strncmp(attr_key, "w", 1)) {
        float val;
        enum llk_ui_unit unit;
        if (0 == parse_length_val(attr_val, &val, &unit)) {
            llk_ui_float_init(elem->w);
            elem->w.specified_val = val;
            elem->w.unit = unit;
            return 0;
        }
    }
    else if (0 == strncmp(attr_key, "h", 1)) {
        float val;
        enum llk_ui_unit unit;
        if (0 == parse_length_val(attr_val, &val, &unit)) {
            llk_ui_float_init(elem->h);
            elem->h.specified_val = val;
            elem->h.unit = unit;
            return 0;
        }
    }
    else if (0 == strncmp(attr_key, "left", 4)) {
        float val;
        enum llk_ui_unit unit;
        if (0 == parse_length_val(attr_val, &val, &unit)) {
            llk_ui_float_init(elem->left);
            elem->left.specified_val = val;
            elem->left.unit = unit;
            return 0;
        }
    }
    else if (0 == strncmp(attr_key, "right", 5)) {
        float val;
        enum llk_ui_unit unit;
        if (0 == parse_length_val(attr_val, &val, &unit)) {
            llk_ui_float_init(elem->right);
            elem->right.specified_val = val;
            elem->right.unit = unit;
            return 0;
        }
    }
    else if (0 == strncmp(attr_key, "top", 3)) {
        float val;
        enum llk_ui_unit unit;
        if (0 == parse_length_val(attr_val, &val, &unit)) {
            llk_ui_float_init(elem->top);
            elem->top.specified_val = val;
            elem->top.unit = unit;
            return 0;
        }
    }
    else if (0 == strncmp(attr_key, "bottom", 6)) {
        float val;
        enum llk_ui_unit unit;
        if (0 == parse_length_val(attr_val, &val, &unit)) {
            llk_ui_float_init(elem->bottom);
            elem->bottom.specified_val = val;
            elem->bottom.unit = unit;
            return 0;
        }
    }

    return -1;
}

struct llk_ui_elem *llk__ui_build_elem_from_xml(struct llk_ui_elem *parent, struct k_xml_node *xml) {

    struct llk_ui_elem *elem = llk_ui_create_elem(parent->ui);
    if (NULL == elem)
        return NULL;

    struct k_xml_node *xml_child;
    for (k_xml_for_each_child(xml, xml_child)) {
        if (K_XML_ELEM_NODE == k_xml_get_type(xml_child)) {
            llk__ui_build_elem_from_xml(elem, xml_child);
        }
    }

    struct k_xml_attr *attr;
    const char *key;
    const char *val;
    for (k_xml_for_each_attr(xml, attr, key, val)) {
        llk__ui_build_elem_attr_from_xml(elem, key, val);
    }

    llk_ui_append_child(parent, elem);

    elem->background_color = rand();

    return elem;
}

struct llk_ui_context *llk__ui_build_from_xml(struct k_xml_node *xml) {

    struct llk_ui_context *ui = llk_ui_create_context();
    if (NULL == ui)
        return NULL;

    struct llk_ui_elem *elem = llk__ui_build_elem_from_xml(ui->root, xml);
    if (NULL == elem) {
        /* TODO free ui */
        return NULL;
    }

    return ui;
}

struct llk_ui_context *llk_ui_build_from_xml_file(const char *file_path) {

    char *text = k_read_txt_file(file_path, NULL, 0, NULL);
    if (NULL == text) {
        k_log_error("Failed to read file");
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

    if (NULL == ui) {
        k_log_error("Failed to create ui context");
        return NULL;
    }

    return ui;
}
