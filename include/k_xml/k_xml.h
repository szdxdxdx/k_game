#ifndef K_XML_H
#define K_XML_H

#include <stddef.h>

int k_xml_parse(char *text);

struct k_xml_node;
struct k_xml_doc;

struct k_xml_node *k_xml_get_root(struct k_xml_doc *doc);

struct k_xml_node *k_xml_get_first_child(struct k_xml_node *node);

struct k_xml_node *k_xml_get_next_sibling(struct k_xml_node *node);

struct k_xml_node *k_xml_get_parent(struct k_xml_node *node);

const char *k_xml_get_tag(struct k_xml_node *elem_node);

const char *k_xml_get_attr(struct k_xml_node *elem_node, const char *attr);

const char *k_xml_get_text(struct k_xml_node *text_node);

#endif
