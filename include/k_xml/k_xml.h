#ifndef K_XML_H
#define K_XML_H

#include <stddef.h>

struct k_xml;

struct k_xml *k_xml_parse(const char *text);

void k_xml_free(struct k_xml *elem);

struct k_xml *k_xml_get_child(struct k_xml *elem, size_t idx);

struct k_xml *k_xml_get_parent(struct k_xml *elem);

const char *k_xml_get_tag(struct k_xml *elem);

const char *k_xml_get_attr(struct k_xml *elem, const char *attr);

#endif
