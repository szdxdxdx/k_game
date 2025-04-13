#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "k_list.h"

#include "k_xml.h"

/* region [struct_def] */

struct k_xml_doc {

    struct k_xml_node *root;
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

/* endregion */

/* region [doc] */

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

void k__xml_doc_destroy_node(struct k_xml_node *node);

void k__xml_destroy_doc(struct k_xml_doc *doc) {
    k__xml_doc_destroy_node(doc->root);
}

/* endregion */

/* region [node] */

struct k_xml_elem_node *k__xml_doc_create_elem_node(struct k_xml_doc *doc, const char *tag) {

    struct k_xml_elem_node *node = k__xml_mem_alloc(doc, sizeof(struct k_xml_elem_node));
    if (NULL == node)
        return NULL;

    k_list_node_loop(&node->base.list_node);
    node->base.doc    = doc;
    node->base.type   = K_XML_ELEM_NODE;
    node->base.parent = NULL;

    node->tag = tag;
    k_list_init(&node->attr_list);
    k_list_init(&node->child_list);

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
    child->parent = &node->base;
    k_list_add_tail(&node->child_list, &child->list_node);
}

void k__xml_doc_destroy_elem_node(struct k_xml_elem_node *node) {

    struct k_xml_doc *doc = node->base.doc;

    {
        struct k_xml_attr *attr;
        struct k_list *attr_list = &node->attr_list;
        struct k_list_node *iter, *next;
        for (k_list_for_each_s(attr_list, iter, next)) {
            attr = container_of(iter, struct k_xml_attr, list_node);

            k__xml_mem_free(doc, attr);
        }
    }

    {
        struct k_xml_node *child;
        struct k_list *child_list = &node->child_list;
        struct k_list_node *iter, *next;
        for (k_list_for_each_s(child_list, iter, next)) {
            child = container_of(iter, struct k_xml_node, list_node);

            k__xml_doc_destroy_node(child);
        }
    }

    k__xml_mem_free(doc, node);
}

struct k_xml_text_node *k__xml_doc_create_text_node(struct k_xml_doc *doc, const char *text) {

    struct k_xml_text_node *node = k__xml_mem_alloc(doc, sizeof(struct k_xml_text_node));
    if (NULL == node)
        return NULL;

    k_list_node_loop(&node->base.list_node);
    node->base.doc    = doc;
    node->base.type   = K_XML_TEXT_NODE;
    node->base.parent = NULL;

    node->text = text;

    return node;
}

void k__xml_doc_destroy_text_node(struct k_xml_text_node *node) {

    struct k_xml_doc *doc = node->base.doc;

    k__xml_mem_free(doc, node);
}

void k__xml_doc_destroy_node(struct k_xml_node *node) {

    switch (node->type) {
        case K_XML_ELEM_NODE: {
            struct k_xml_elem_node *elem_node = container_of(node, struct k_xml_elem_node, base);
            k__xml_doc_destroy_elem_node(elem_node);
            break;
        }
        case K_XML_TEXT_NODE: {
            struct k_xml_text_node *text_node = container_of(node, struct k_xml_text_node, base);
            k__xml_doc_destroy_text_node(text_node);
            break;
        }
        default:
            assert(0);
    }
}

/* endregion */

/* region [parser] */

struct k_xml_parser {

    char *p;

    struct k_xml_doc *doc;
};

static char *skip_space(char *p) {
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    return p;
}

static char *extract_ident(char *text) {

    char *begin = text;
    if ( ! isalpha((unsigned char)*begin) && '_' != *begin)
        goto err;

    char *end = begin + 1;
    while (isalnum((unsigned char)*end) || '_' == *end) {
        end++;
    }

    return end;

err:
    return text;
}

static char *extract_string(char *text) {

    char *begin = text;
    if ('\"' != *begin && '\'' != *begin)
        goto err;

    int entities_count = 0;
    char *end = begin + 1;
    while (1) {

        if (*begin == *end) {
            break;
        }
        else if ('\0' == *end) {
            goto err;
        }
        else if ('&' == *end) {
            for (end++; *end != ';'; end++) {
                if ('&' == *end || '\0' == *end)
                    goto err;
            }
            entities_count += 1;
        }
        else {
            end++;
        }
    }

    if (0 < entities_count) {


    }

    return end;

err:
    return text;
}

struct k_xml_elem_node *k__xml_parse(struct k_xml_parser *parser) {

    if ('<' != *(parser->p))
        return NULL;

    char *p = parser->p;

    char *elem_begin = p;

    char *tag = p + 1;
    char *tag_end = extract_ident(tag);
    if (tag == tag_end)
        return NULL;

    struct k_xml_elem_node *elem = k__xml_doc_create_elem_node(parser->doc, tag);
    if (NULL == elem)
        return NULL;

    p = tag_end;

    while(1) {
        p = skip_space(p);

        if ('>' == *p) {
            p += 1;
            break;
        }
        else if ('/' == *p) {
            if ('>' != *(p + 1)) {
                goto err;
            } else {
                parser->p = p + 2;
                p += 2;
                goto done;
            }
        }

        char *key = p;
        char *key_end = extract_ident(key);
        if (key == key_end)
            goto err;

        p = skip_space(key_end);
        if ('=' != *p)
            goto err;

        char *val = skip_space(p + 1);
        char *val_end = extract_string(val);
        if (val == val_end)
            goto err;

        *key_end = '\0';
        *val_end = '\0';
        k__xml_elem_node_add_attr(elem, key, val + 1);

        p = val_end + 1;
    }

    *tag_end = '\0';

    while (1) {
        if ('<' == *p) {
            if ('/' == *(p + 1)) {
                size_t tag_len = tag_end - tag;
                if (0 != strncmp(tag, p + 2, tag_len))
                    goto err;

                *p = '\0';
                p = skip_space(p + 2 + tag_len);
                if ('>' != *p)
                    goto err;

                p += 1;
                goto done;
            }
            else {
                parser->p = p;
                struct k_xml_elem_node *elem_child = k__xml_parse(parser);
                if (NULL == elem_child)
                    goto err;

                k__xml_elem_node_add_child(elem, &elem_child->base);
                p = parser->p;
            }
        }
        else {
            char *text = p;
            p += 1;
            while ('<' != *p) {
                if ('\0' == *p)
                    goto err;
                else
                    p++;
            }

            char *text_end = p;

            if (text != text_end) {

                struct k_xml_text_node *text_node = k__xml_doc_create_text_node(parser->doc, text);
                if (NULL == text_node)
                    goto err;

                k__xml_elem_node_add_child(elem, &text_node->base);
            }
        }
    }

done:
    *elem_begin= '\0';
    parser->p = p;
    return elem;

err:
    k__xml_doc_destroy_elem_node(elem);
    return NULL;
}

/* endregion */

/* region [print] */

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define CLEAR   "\033[0m"

void k__xml_print(struct k_xml_node *node) {

    if (NULL == node) {
        printf("(null)");
        return;
    }

    if (K_XML_ELEM_NODE == node->type) {
        struct k_xml_elem_node *elem_node = container_of(node, struct k_xml_elem_node, base);

        printf("<" MAGENTA "%s" CLEAR, elem_node->tag);

        struct k_list_node *attr_iter;
        for (k_list_for_each(&elem_node->attr_list, attr_iter)) {
            struct k_xml_attr *attr = container_of(attr_iter, struct k_xml_attr, list_node);

            printf(" " GREEN "%s" CLEAR "=\"" YELLOW "%s" CLEAR "\"", attr->key, attr->val);
        }

        printf(">");

        struct k_list_node *child_iter;
        for (k_list_for_each(&elem_node->child_list, child_iter)) {
            struct k_xml_node *child = container_of(child_iter, struct k_xml_node, list_node);

            k__xml_print(child);
        }

        printf("</" MAGENTA "%s" CLEAR ">", elem_node->tag);
    }
    else if (K_XML_TEXT_NODE == node->type) {
        struct k_xml_text_node *text_node = container_of(node, struct k_xml_text_node, base);
        printf(RED "%s" CLEAR, text_node->text);
    }
}

/* endregion */

/* region [k_xml] */

struct k_xml_node *k_xml_parse(char *text) {

    if (NULL == text || '\0' == *text)
        return NULL;

    struct k_xml_parser parser;
    parser.doc = k__xml_create_doc();
    parser.p = skip_space(text);

    if ('<' != *(parser.p))
        goto err;

    struct k_xml_elem_node *elem = k__xml_parse(&parser);
    if (NULL == elem)
        goto err;

    if ('\0' != *(skip_space(parser.p))) {
        k__xml_doc_destroy_elem_node(elem);
        goto err;
    }

    parser.doc->root = &elem->base;
    return parser.doc->root;

err:
    return NULL;
}

void k_xml_free(struct k_xml_node *node) {

    if (NULL == node)
        return;

    k__xml_destroy_doc(node->doc);
}

struct k_xml_node *k_xml_get_first_child(struct k_xml_node *node) {

    if (K_XML_ELEM_NODE == node->type) {
        struct k_xml_elem_node *elem = container_of(node, struct k_xml_elem_node, base);

        struct k_list *child_list = &elem->child_list;
        if (k_list_is_empty(child_list))
            return NULL;

        struct k_list_node *list_node = k_list_get_first(child_list);
        struct k_xml_node *child = container_of(list_node, struct k_xml_node, list_node);

        return child;
    }
    else {
        return NULL;
    }
}

struct k_xml_node *k_xml_get_next_sibling(struct k_xml_node *node) {

    struct k_xml_node *parent = node->parent;
    if (NULL == parent)
        return NULL;

    struct k_xml_elem_node *elem = container_of(parent, struct k_xml_elem_node, base);
    struct k_list *parent_child_list = &elem->child_list;

    struct k_list_node *next = node->list_node.next;
    if (next == &parent_child_list->head)
        return NULL;

    struct k_xml_node *sibling = container_of(next, struct k_xml_node, list_node);
    return sibling;
}

struct k_xml_node *k_xml_get_parent(struct k_xml_node *node) {
    return node->parent;
}

struct k_xml_node *k_xml_find_child_by_tag(struct k_xml_node *node, const char *tag) {

    if (K_XML_ELEM_NODE != node->type)
        return NULL;
    if (NULL == tag || '\0' == *tag)
        return NULL;

    struct k_xml_elem_node *elem = container_of(node, struct k_xml_elem_node, base);

    struct k_list *child_list = &elem->child_list;
    if (k_list_is_empty(child_list))
        return NULL;

    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        struct k_xml_node *child = container_of(iter, struct k_xml_node, list_node);

        if (K_XML_ELEM_NODE == child->type) {
            struct k_xml_elem_node *child_elem = container_of(child, struct k_xml_elem_node, base);

            if (0 == strcmp(child_elem->tag, tag))
                return child;
        }
    }

    return NULL;
}

struct k_xml_node *k_xml_find_next_by_tag(struct k_xml_node *node, const char *tag) {

    struct k_xml_node *parent = node->parent;
    if (NULL == parent)
        return NULL;

    struct k_xml_elem_node *elem = container_of(parent, struct k_xml_elem_node, base);

    struct k_xml_node *sibling;
    struct k_list *parent_child_list = &elem->child_list;
    struct k_list_node *iter = node->list_node.next;
    for (; iter != &parent_child_list->head; iter = iter->next) {
        sibling = container_of(iter, struct k_xml_node, list_node);

        if (K_XML_ELEM_NODE == sibling->type) {
            struct k_xml_elem_node *sibling_elem = container_of(sibling, struct k_xml_elem_node, base);

            if (0 == strcmp(sibling_elem->tag, tag))
                return sibling;
        }
    }

    return NULL;
}

enum k_xml_node_type k_xml_get_type(struct k_xml_node *node) {
    return node->type;
}

const char *k_xml_get_tag(struct k_xml_node *elem_node) {

    if (K_XML_ELEM_NODE != elem_node->type)
        return NULL;

    struct k_xml_elem_node *elem = container_of(elem_node, struct k_xml_elem_node, base);
    return elem->tag;
}

const char *k_xml_get_attr(struct k_xml_node *elem_node, const char *attr) {

    if (K_XML_ELEM_NODE != elem_node->type)
        return NULL;
    if (NULL == attr || '\0' == *attr)
        return NULL;

    struct k_xml_elem_node *elem = container_of(elem_node, struct k_xml_elem_node, base);

    struct k_xml_attr *attr_node;
    struct k_list *attr_list = &elem->attr_list;
    struct k_list_node *iter;
    for (k_list_for_each(attr_list, iter)) {
        attr_node = container_of(iter, struct k_xml_attr, list_node);

        if (0 == strcmp(attr_node->key, attr))
            return attr_node->val;
    }

    return NULL;
}

const char *k_xml_get_text(struct k_xml_node *text_node) {

    if (K_XML_TEXT_NODE == text_node->type) {
        struct k_xml_text_node *text = container_of(text_node, struct k_xml_text_node, base);
        return text->text;
    }
    else if (K_XML_ELEM_NODE == text_node->type) {
        struct k_xml_elem_node *elem = container_of(text_node, struct k_xml_elem_node, base);

        struct k_list *child_list = &elem->child_list;
        if (k_list_is_empty(child_list))
            return NULL;

        struct k_xml_node *child;
        struct k_list_node *iter;
        for (k_list_for_each(child_list, iter)) {
            child = container_of(iter, struct k_xml_node, list_node);

            if (K_XML_TEXT_NODE == child->type) {
                struct k_xml_text_node *text = container_of(child, struct k_xml_text_node, base);
                return text->text;
            }
        }

        return NULL;
    }
    else {
        return NULL;
    }
}

/* endregion */
