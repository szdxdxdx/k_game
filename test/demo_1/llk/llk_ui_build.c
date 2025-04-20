#include <stdlib.h>

#include "k_read_file.h"
#include "k_xml.h"
#include "k_log.h"

#include "./llk_ui_build.h"
#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"

struct llk_ui_elem *llk__ui_build_elem_from_xml(struct llk_ui_elem *parent, struct k_xml_node *xml) {

    const char *tag = k_xml_get_tag(xml);

    struct llk_ui_elem *elem = llk_ui_create_elem(parent->ui, tag);
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

        if (0 != llk_ui_elem_set_attr(elem, key, val)) {
            k_log_warn("Illegal attribute when building llk UI element, key: `%s`, value: `%s`", key, val);
        }
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
        goto err;
    }

    struct k_xml_node *xml = k_xml_parse(text);

    if (NULL == xml) {
        free(text);
        k_log_error("Failed to parse xml");
        goto err;
    }

    struct llk_ui_context *ui = llk__ui_build_from_xml(xml);

    k_xml_free(xml);
    free(text);

    return ui;

err:
    k_log_error("Failed to build llk UI from xml file");
    return NULL;
}
