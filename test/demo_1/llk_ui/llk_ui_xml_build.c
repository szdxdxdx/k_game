#include <stdlib.h>

#include "k_read_file.h"
#include "k_xml.h"

#define K_LOG_TAG "llk:UI"
#include "k_log.h"

#include "./llk_ui_ext.h"

struct llk_ui_elem *llk__ui_build_elem_from_xml(struct llk_ui_context *ui, struct k_xml_node *xml) {

    const char *tag = k_xml_get_tag(xml);

    struct llk_ui_elem *elem = llk_ui_create_elem(ui, tag);
    if (NULL == elem)
        return NULL;

    struct k_xml_node *xml_child;
    for (k_xml_for_each_child(xml, xml_child)) {
        if (K_XML_ELEM_NODE == k_xml_get_type(xml_child)) {

            struct llk_ui_elem *child = llk__ui_build_elem_from_xml(ui, xml_child);

            if (NULL == child)
                goto err;

            llk_ui_append_child(elem, child);
        }
    }

    struct k_xml_attr *attr;
    const char *key;
    const char *val;
    for (k_xml_for_each_attr(xml, attr, key, val)) {

        llk_ui_elem_set_attr(elem, key, val);
    }

    return elem;

err:
    k_log_error("Failed to build element from xml node, node tag: `%s`", tag);
    /* TODO if build failed, destroy elem */
    return NULL;
}

struct llk_ui_context *llk__ui_build_from_xml(struct k_xml_node *xml) {

    struct llk_ui_context *ui = llk_ui_create_context();
    if (NULL == ui)
        return NULL;

    struct llk_ui_elem *elem = llk__ui_build_elem_from_xml(ui, xml);
    if (NULL == elem) {
        llk_ui_destroy_context(ui);
        k_log_error("Failed to build UI from xml");
        return NULL;
    }

    llk_ui_append_child(llk_ui_get_root(ui), elem);
    return ui;
}

struct llk_ui_elem *llk_ui_build_elem_from_xml_file(struct llk_ui_context *ui, const char *file_path) {

    char *text = k_read_txt_file(file_path, NULL, 0, NULL);
    if (NULL == text) {
        k_log_error("Failed to read file `%s`", file_path);
        goto err;
    }

    struct k_xml_node *xml = k_xml_parse_in_place(text);

    if (NULL == xml) {
        k_log_error("Failed to parse xml");
        goto err;
    }

    struct llk_ui_elem *elem = llk__ui_build_elem_from_xml(ui, xml);

    k_xml_free(xml);
    free(text);

    return elem;

err:
    k_log_error("Failed to build UI from xml file");
    return NULL;
}
