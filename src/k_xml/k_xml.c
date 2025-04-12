#include "k_xml.h"

struct k_xml_doc {

    struct k_xml *root;

    struct k_xml *elems;
    size_t elems_num;
};

enum k_xml_type {
    K__XML_ELEM,
    K__XML_TEXT,
};

struct k_xml_attr {
    const char *key;
    const char *val;
};

struct k_xml_elem {

    struct k_xml *children;
    size_t children_count;

    char *tag;

    struct k_xml_attr *attr;
    size_t attr_count;
};

struct k_xml_text {
    char *text;
    size_t len;
};

struct k_xml {

    struct k_xml_doc *doc;

    struct k_xml *parent;

    enum k_xml_type type;
    union {
        struct k_xml_elem elem;
        struct k_xml_text text;
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

/* region [api] */

struct k_xml *k_xml_parse(const char *text) {

    struct k_xml_parser parser;

    return NULL;
}

void k_xml_free(struct k_xml *elem) {

}

struct k_xml *k_xml_get_child(struct k_xml *elem, size_t idx) {
    return NULL;
}

struct k_xml *k_xml_get_parent(struct k_xml *elem) {
    return NULL;
}

const char *k_xml_get_tag(struct k_xml *elem) {
    return NULL;
}

const char *k_xml_get_attr(struct k_xml *elem, const char *attr) {
    return NULL;
}

/* endregion */
