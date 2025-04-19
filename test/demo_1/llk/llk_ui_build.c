#include <stdlib.h>
#include <stdio.h>

#include "k_read_file.h"
#include "k_xml.h"
#include "k_log.h"

#include "./llk_ui_build.h"
#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"

int llk__ui_build_elem_attr_from_xml(struct llk_ui_elem *elem, const char *key, const char *val) {

    printf("key: %s, val: %s\n", key, val);
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
