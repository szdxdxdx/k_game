#include "k_xml.h"

struct k_xml_doc {

    struct k_xml *root;

    struct k_xml *elems;
    size_t elems_num;
};

enum k_xml_node_type {
    K__XML_ELEM_NODE,
    K__XML_TEXT_NODE,
};

struct k_xml_attr {
    struct k_xml_attr *next_attr;
    const char *key;
    const char *val;
};

struct k_xml_elem_node {
    enum k_xml_node_type type;

    struct k_xml_node *parent;
    struct k_xml_node *first_child;
    struct k_xml_node *next_sibling;

    char *tag;

    struct k_xml_attr *first_attr;
};

struct k_xml_text_node {
    enum k_xml_node_type type;

    struct k_xml_node *parent;

    char *text;
    size_t len;
};

struct k_xml_node {
    union {
        enum k_xml_node_type type;
        struct k_xml_elem_node elem;
        struct k_xml_text_node text;
    };
};

struct k_xml_parser {

    const char *text;
    size_t cursor;

    struct k_xml *elems_buf;
    size_t buf_size;
    size_t buf_capacity;

    int depth;

    struct k_xml_doc *doc;
};
