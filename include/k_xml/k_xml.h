#ifndef K_XML_H
#define K_XML_H

#include <stddef.h>

struct k_xml_node;

enum k_xml_node_type {
    K_XML_ELEM_NODE,
    K_XML_TEXT_NODE,
};

struct k_xml_node *k_xml_parse(char *text);

void k_xml_free(struct k_xml_node *node);

struct k_xml_node *k_xml_get_first_child(struct k_xml_node *node);

struct k_xml_node *k_xml_get_next_sibling(struct k_xml_node *node);

struct k_xml_node *k_xml_get_parent(struct k_xml_node *node);

struct k_xml_node *k_xml_find_child_by_tag(struct k_xml_node *node, const char *tag);

struct k_xml_node *k_xml_find_next_by_tag(struct k_xml_node *node, const char *tag);

enum k_xml_node_type k_xml_get_type(struct k_xml_node *node);

const char *k_xml_get_tag(struct k_xml_node *elem_node);

const char *k_xml_get_attr(struct k_xml_node *elem_node, const char *attr);

const char *k_xml_get_text(struct k_xml_node *text_node);

#endif
