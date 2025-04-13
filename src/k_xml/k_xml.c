#include <stdlib.h>
#include <stdio.h>

#include "k_list.h"

#include "k_xml.h"

struct k_xml_doc {

    struct k_xml *root;
};

enum k_xml_node_type {
    K__XML_ELEM_NODE,
    K__XML_TEXT_NODE,
};

struct k_xml_node {
    struct k_list_node list_node;
    struct k_xml_doc *doc;
    enum k_xml_node_type type;
    struct k_xml_node *parent;
};

struct k_xml_elem_node {
    struct k_xml_node base;

    const char *tag;
    struct k_list attr_list;
    struct k_list child_list;
};

struct k_xml_attr {
    struct k_list_node list_node;
    const char *key;
    const char *val;
};

struct k_xml_text_node {
    struct k_xml_node base;
    const char *text;
};

void *k__xml_mem_alloc(struct k_xml_doc *doc, size_t size) {
    (void)doc;
    return malloc(size);
}

void k__xml_mem_free(struct k_xml_doc *doc, void *p) {
    (void)doc;
    free(p);
}

struct k_xml_doc *k__xml_create_doc(void) {

    struct k_xml_doc *doc = malloc(sizeof(struct k_xml_doc));
    if (NULL == doc)
        return NULL;

    return doc;
}

struct k_xml_elem_node *k__xml_doc_create_elem_node(struct k_xml_doc *doc, const char *tag) {

    struct k_xml_elem_node *node = k__xml_mem_alloc(doc, sizeof(struct k_xml_elem_node));
    if (NULL == node)
        return NULL;

    k_list_node_loop(&node->base.list_node);
    node->base.doc    = doc;
    node->base.type   = K__XML_ELEM_NODE;
    node->base.parent = NULL;

    node->tag = tag;
    k_list_init(&node->attr_list);
    k_list_init(&node->child_list);

    return node;
}

struct k_xml_text_node *k__xml_doc_create_text_node(struct k_xml_doc *doc, const char *text) {

    struct k_xml_text_node *node = k__xml_mem_alloc(doc, sizeof(struct k_xml_text_node));
    if (NULL == node)
        return NULL;

    k_list_node_loop(&node->base.list_node);
    node->base.doc    = doc;
    node->base.type   = K__XML_TEXT_NODE;
    node->base.parent = NULL;

    node->text = text;

    return node;
}

int k__xml_elem_node_add_attr(struct k_xml_elem_node *node, const char *key, const char *val) {

    struct k_xml_attr *attr = k__xml_mem_alloc(node->base.doc, sizeof(struct k_xml_attr));
    if (NULL == attr)
        return -1;

    k_list_add_tail(&node->attr_list, &attr->list_node);
    attr->key = key;
    attr->val = val;
    return 0;
}

void k__xml_elem_node_add_child(struct k_xml_elem_node *node, struct k_xml_node *child) {
    k_list_add_tail(&node->child_list, &child->list_node);
}

void k__xml_print(struct k_xml_node *node) {

    if (K__XML_ELEM_NODE == node->type) {
        struct k_xml_elem_node *elem_node = container_of(node, struct k_xml_elem_node, base);

        printf("<%s", elem_node->tag);

        struct k_list_node *attr_iter;
        for (k_list_for_each(&elem_node->attr_list, attr_iter)) {
            struct k_xml_attr *attr = container_of(attr_iter, struct k_xml_attr, list_node);

            printf(" %s=\"%s\"", attr->key, attr->val);
        }

        printf(">");

        struct k_list_node *child_iter;
        for (k_list_for_each(&elem_node->child_list, child_iter)) {
            struct k_xml_node *child = container_of(child_iter, struct k_xml_node, list_node);

            k__xml_print(child);
        }

        printf("</%s>", elem_node->tag);
    }
    else if (K__XML_TEXT_NODE == node->type) {
        struct k_xml_text_node *text_node = container_of(node, struct k_xml_text_node, base);
        printf("%s", text_node->text);
    }
}

int main(void) {

    struct k_xml_doc *doc = k__xml_create_doc();

    struct k_xml_elem_node *window = k__xml_doc_create_elem_node(doc, "window");
    k__xml_elem_node_add_attr(window, "x", "100");
    k__xml_elem_node_add_attr(window, "y", "100");

    struct k_xml_text_node *title  = k__xml_doc_create_text_node(doc, "title");
    k__xml_elem_node_add_child(window, &title->base);

    struct k_xml_elem_node *button = k__xml_doc_create_elem_node(doc, "button");
    struct k_xml_text_node *click_me  = k__xml_doc_create_text_node(doc, "click me");
    k__xml_elem_node_add_child(button, &click_me->base);
    k__xml_elem_node_add_child(window, &button->base);

    k__xml_print(&window->base);

    return 0;
}
