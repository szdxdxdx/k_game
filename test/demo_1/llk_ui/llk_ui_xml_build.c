#include <stdlib.h>

#include "k_read_file.h"
#include "k_xml.h"

#define K_LOG_TAG "llk:UI"
#include "k_log.h"

#include "./llk_ui_ext.h"

struct llk_ui_elem *llk__ui_build_elem_from_xml(struct llk_ui_context *ui, struct k_xml_node *xml) {

    const char *tag = k_xml_get_tag(xml); /* 获取 xml 节点的 tag，根据该 tag 创建 UI 元素实例 */
    struct llk_ui_elem *elem = llk_ui_elem_create(ui, tag);
    if (NULL == elem) return NULL;

    struct k_xml_node *xml_child; /* 遍历该 xml 节点的子节点 */
    for (k_xml_for_each_child(xml, xml_child)) {
        if (K_XML_ELEM_NODE == k_xml_get_type(xml_child)) {  /* 如果子节点是元素节点，则递归创建 UI 元素 */
            struct llk_ui_elem *child = llk__ui_build_elem_from_xml(ui, xml_child);
            if (NULL == child)
                goto err;
            llk_ui_elem_append_child(elem, child); /* 给当前 UI 元素添加子元素 */
        }
    }
    struct k_xml_attr *attr; /* 遍历该 xml 节点的属性 */
    const char *key, *val;
    for (k_xml_for_each_attr(xml, attr, &key, &val)) {
        llk_ui_elem_set_attr(elem, key, val); /* 给 UI 元素的属性赋值 */
    }
    return elem;

err:
    k_log_error("failed to build element from xml node, node tag: `%s`", tag);
    llk_ui_elem_destroy(elem);
    return NULL;
}

struct llk_ui_elem *llk_ui_build_elem_from_xml_file(struct llk_ui_context *ui, const char *file_path) {

    if (NULL == ui) {
        k_log_error("ui context is null");
        goto err;
    }
    if (NULL == file_path || '\0' == file_path[0]) {
        k_log_error("file path is empty");
        goto err;
    }

    char *text = k_read_txt_file(file_path, NULL, 0, NULL);
    if (NULL == text) {
        k_log_error("failed to read file `%s`", file_path);
        goto err;
    }

    struct k_xml_node *xml = k_xml_parse_in_place(text);
    if (NULL == xml) {
        free(text);
        k_log_error("failed to parse xml");
        goto err;
    }

    struct llk_ui_elem *elem = llk__ui_build_elem_from_xml(ui, xml);

    k_xml_free(xml);
    free(text);

    if (NULL == elem) {
        k_log_error("failed to build UI from xml file");
        goto err;
    }

    return elem;

err:
    return NULL;
}
