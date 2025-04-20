#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "k_list.h"
#include "k_mem_pool.h"

#include "k_xml.h"

/* region [struct_def] */

struct k_xml_elem_node;

struct k_xml_doc {

    /* 用链表串联所有的顶层节点（可能包含注释节点） */
    struct k_list top_node_list;

    /* 指向根节点 */
    struct k_xml_node *root;

    struct k_mem_pool mem_pool;
};

struct k_xml_node {

    struct k_list_node sibling_link;

    struct k_xml_node *parent;

    struct k_xml_doc *doc;

    enum k_xml_node_type type;
};

struct k_xml_elem_node {
    struct k_xml_node base;

    const char *tag;

    /* 用链表串联所有的属性 */
    struct k_list attr_list;

    /* 用链表串联所有的孩子节点 */
    struct k_list child_list;
};

struct k_xml_attr {

    /* 属性链表节点的指针域 */
    struct k_list_node list_node;

    /* 该属性所属的元素节点 */
    struct k_xml_elem_node *elem;

    /* 属性的键和值 */
    const char *key;
    const char *val;
};

struct k_xml_text_node {
    struct k_xml_node base;

    /* 文本内容 */
    const char *text;

    /* 标记文本内容是否全是空白字符 */
    int is_blank;
};

struct k_xml_comment_node {
    struct k_xml_node base;

    /* 注释的文本内容 */
    const char *comment;
};

/* endregion */

/* region */

static struct k_xml_elem_node *k__xml_create_elem_node(struct k_xml_doc *doc, const char *tag);

static void k__xml_destroy_node(struct k_xml_node *node);

/* endregion */

/* region [doc] */

static void *k__xml_mem_alloc(struct k_xml_doc *doc, size_t size) {
    return k_mem_pool_alloc(&doc->mem_pool, size);
}

static void k__xml_mem_free(struct k_xml_doc *doc, void *p) {
    k_mem_pool_free(p);
}

static struct k_xml_doc *k__xml_create_doc(void) {

    struct k_xml_doc *doc = malloc(sizeof(struct k_xml_doc));
    if (NULL == doc)
        return NULL;

    struct k_mem_pool_config config;
    config.fn_malloc        = malloc;
    config.fn_free          = free;
    config.alloc_size_align = 8;
    config.block_size_max   = 80;
    config.alloc_chunk_size = 2048;

    if (NULL == k_mem_pool_construct(&doc->mem_pool, &config))
        return NULL;

    k_list_init(&doc->top_node_list);
    doc->root = NULL;

    return doc;
}

static void k__xml_destroy_doc(struct k_xml_doc *doc) {

    struct k_xml_node *xml_node;
    struct k_list *list = &doc->top_node_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        xml_node = container_of(iter, struct k_xml_node, sibling_link);

        k__xml_destroy_node(xml_node);
    }

    k_mem_pool_destruct(&doc->mem_pool);
    free(doc);
}

/* endregion */

/* region [node] */

/* region [elem_node] */

static struct k_xml_elem_node *k__xml_create_elem_node(struct k_xml_doc *doc, const char *tag) {

    struct k_xml_elem_node *node = k__xml_mem_alloc(doc, sizeof(struct k_xml_elem_node));
    if (NULL == node)
        return NULL;

    k_list_node_loop(&node->base.sibling_link);
    node->base.doc    = doc;
    node->base.type   = K_XML_ELEM_NODE;
    node->base.parent = NULL;

    node->tag = tag;
    k_list_init(&node->attr_list);
    k_list_init(&node->child_list);

    return node;
}

static int k__xml_elem_node_add_attr(struct k_xml_elem_node *node, const char *key, const char *val) {

    struct k_xml_attr *attr = k__xml_mem_alloc(node->base.doc, sizeof(struct k_xml_attr));
    if (NULL == attr)
        return -1;

    k_list_add_tail(&node->attr_list, &attr->list_node);
    attr->elem = node;
    attr->key  = key;
    attr->val  = val;
    return 0;
}

static void k__xml_elem_node_add_child(struct k_xml_elem_node *node, struct k_xml_node *child) {
    child->parent = &node->base;
    k_list_add_tail(&node->child_list, &child->sibling_link);
}

static void k__xml_destroy_elem_node(struct k_xml_elem_node *node) {

    struct k_xml_doc *doc = node->base.doc;

    struct k_xml_attr *attr;
    struct k_list *attr_list = &node->attr_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(attr_list, iter, next)) {
        attr = container_of(iter, struct k_xml_attr, list_node);

        k__xml_mem_free(doc, attr);
    }

    struct k_xml_node *child;
    struct k_list *child_list = &node->child_list;
    struct k_list_node *iter_, *next_;
    for (k_list_for_each_s(child_list, iter_, next_)) {
        child = container_of(iter_, struct k_xml_node, sibling_link);

        k__xml_destroy_node(child);
    }

    k_list_del(&node->base.sibling_link);
    k__xml_mem_free(doc, node);
}

/* endregion */

/* region [text_node] */

static struct k_xml_text_node *k__xml_create_text_node(struct k_xml_doc *doc) {

    struct k_xml_text_node *node = k__xml_mem_alloc(doc, sizeof(struct k_xml_text_node));
    if (NULL == node)
        return NULL;

    k_list_node_loop(&node->base.sibling_link);
    node->base.doc    = doc;
    node->base.type   = K_XML_TEXT_NODE;
    node->base.parent = NULL;

    node->text = "";
    node->is_blank = 1;

    return node;
}

static void k__xml_destroy_text_node(struct k_xml_text_node *node) {
    k_list_del(&node->base.sibling_link);
    k__xml_mem_free(node->base.doc, node);
}

/* endregion */

/* region [comment_node] */

static struct k_xml_comment_node *k__xml_create_comment_node(struct k_xml_doc *doc, const char *comment) {

    struct k_xml_comment_node *node = k__xml_mem_alloc(doc, sizeof(struct k_xml_comment_node));
    if (NULL == node)
        return NULL;

    k_list_node_loop(&node->base.sibling_link);
    node->base.doc    = doc;
    node->base.type   = K_XML_COMMENT_NODE;
    node->base.parent = NULL;

    node->comment = comment;

    return node;
}

static void k__xml_destroy_comment_node(struct k_xml_comment_node *node) {
    k_list_del(&node->base.sibling_link);
    k__xml_mem_free(node->base.doc, node);
}

/* endregion */

static void k__xml_destroy_node(struct k_xml_node *node) {

    switch (node->type) {
        case K_XML_ELEM_NODE: {
            struct k_xml_elem_node *elem_node = container_of(node, struct k_xml_elem_node, base);
            k__xml_destroy_elem_node(elem_node);
            break;
        }
        case K_XML_TEXT_NODE: {
            struct k_xml_text_node *text_node = container_of(node, struct k_xml_text_node, base);
            k__xml_destroy_text_node(text_node);
            break;
        }
        case K_XML_COMMENT_NODE: {
            struct k_xml_comment_node *comment_node = container_of(node, struct k_xml_comment_node, base);
            k__xml_destroy_comment_node(comment_node);
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
    char *p = text;

    if (! isalpha((unsigned char)*p) && '_' != *p)
        goto err;

    p++;
    while (isalnum((unsigned char)*p) || '_' == *p || '-' == *p) {
        p++;
    }

    return p;

err:
    return text;
}

static int decode_entities(char *begin, const char *end, size_t *get_len) {

    char *p1 = NULL;
    char *p2 = begin;

    while (p2 < end) {
        if ('&' != *p2) {
            p2++;
        } else {
            p1 = p2;
            p2++;
            if (0 == strncmp(p2, "amp;",  4)) { *p1 = '&';  p2 += 4; break; }
            if (0 == strncmp(p2, "lt;",   3)) { *p1 = '<';  p2 += 3; break; }
            if (0 == strncmp(p2, "gt;",   3)) { *p1 = '>';  p2 += 3; break; }
            if (0 == strncmp(p2, "quot;", 5)) { *p1 = '\"'; p2 += 5; break; }
            if (0 == strncmp(p2, "apos;", 5)) { *p1 = '\''; p2 += 5; break; }
            goto err;
        }
    }

    p1++;

    while (p2 < end) {
        if ('&' != *p2) {
            *p1 = *p2;
            p1++;
            p2++;
        } else {
            p2++;
            if (0 == strncmp(p2, "amp;",  4)) { *p1 = '&';  p1 += 1; p2 += 4; continue; }
            if (0 == strncmp(p2, "lt;",   3)) { *p1 = '<';  p1 += 1; p2 += 3; continue; }
            if (0 == strncmp(p2, "gt;",   3)) { *p1 = '>';  p1 += 1; p2 += 3; continue; }
            if (0 == strncmp(p2, "quot;", 5)) { *p1 = '\"'; p1 += 1; p2 += 5; continue; }
            if (0 == strncmp(p2, "apos;", 5)) { *p1 = '\''; p1 += 1; p2 += 5; continue; }
            goto err;
        }
    }

    *p1 = '\0';

    if (NULL != get_len) {
        *get_len = p1 - begin;
    }

    return 0;

err:
    return -1;
}

static char *extract_string(char *text) {

    char *p = text;

    if ('\"' != *p && '\'' != *p)
        goto err;

    char *quote = p;
    p++;
    int has_entities = 0;
    while (1) {
        if (*quote == *p) {
            break;
        } else if ('\0' == *p) {
            goto err;
        } else if ('&' == *p) {
            p++;
            has_entities = 1;
        } else {
            p++;
        }
    }

    if (has_entities) {
        if (0 != decode_entities(quote, p, NULL))
            goto err;
    } else {
        *p = '\0';
    }

    return p;

err:
    return text;
}

static struct k_xml_node *k__xml_parse_node(struct k_xml_parser *parser);

static struct k_xml_text_node *k__xml_parse_text_node(struct k_xml_parser *parser) {

    char *p = parser->p;

    if ('<' == *p)
        return NULL;

    char *text_begin = p;

    int has_entities = 0;
    int is_blank = 1;
    while ('<' != *p && '\0' != *p) {

        if ( ! isspace((unsigned char)*p)) {
            is_blank = 0;

            if ('&' == *p) {
                has_entities = 1;
            }
        }

        p++;
    }

    size_t text_len;

    if (has_entities) {
        if (0 != decode_entities(text_begin, p, &text_len))
            goto err;
    }

    struct k_xml_text_node *text = k__xml_create_text_node(parser->doc);
    if (NULL == text)
        goto err;

    text->text = text_begin;
    text->is_blank = is_blank;

    parser->p = p;
    return text;

err:
    return NULL;
}

static struct k_xml_comment_node *k__xml_parse_comment_node(struct k_xml_parser *parser) {

    char *p = parser->p;

    char *comment_begin = p;

    if ('<' != *p) goto err; else p++;
    if ('!' != *p) goto err; else p++;
    if ('-' != *p) goto err; else p++;
    if ('-' != *p) goto err; else p++;

    char *comment_text_begin = p;

    while (1) {
        if ('\0' == *p) {
            goto err;
        }
        else if ('-' == *p) {
            p++;
            if ('-' == *p) {
                p++;
                if ('>' == *p) {
                    p++;
                    *(p - 3) = '\0';
                    break;
                }
                else {
                    goto err;
                }
            }
        }
        else {
            p++;
        }
    }

    struct k_xml_comment_node *comment_node = k__xml_create_comment_node(parser->doc, comment_text_begin);
    if (NULL == comment_node)
        goto err;

    *comment_begin = '\0';
    parser->p = p;

    return comment_node;

err:
    return NULL;
}

static struct k_xml_elem_node *k__xml_parse_elem_node(struct k_xml_parser *parser) {

    char *p = parser->p;

    if ('<' != *(parser->p))
        return NULL;

    char *elem_begin = p;

    char *tag = p + 1;
    char *tag_end = extract_ident(tag);
    if (tag == tag_end)
        return NULL;

    struct k_xml_elem_node *elem = k__xml_create_elem_node(parser->doc, tag);
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
                *p = '\0';
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

        *key_end = '\0';

        char *val = skip_space(p + 1);
        char *val_end = extract_string(val);
        if (val == val_end)
            goto err;

        val++;
        *val_end = '\0';

        k__xml_elem_node_add_attr(elem, key, val);

        p = val_end + 1;
    }

    while (1) {
        if ('<' == *p && '/' == *(p + 1)) {
            char *close_tag = p;
            p += 2;
            size_t tag_len = tag_end - tag;
            if (0 != strncmp(tag, p, tag_len))
                goto err;

            p += tag_len;
            p = skip_space(p);
            if ('>' != *p)
                goto err;

            *close_tag = '\0';
            p += 1;
            goto done;
        }

        parser->p = p;
        struct k_xml_node *child = k__xml_parse_node(parser);
        if (NULL == child)
            goto err;

        k__xml_elem_node_add_child(elem, child);
        p = parser->p;
    }

done:
    *elem_begin= '\0';
    *tag_end = '\0';
    parser->p = p;
    return elem;

err:
    k__xml_destroy_elem_node(elem);
    return NULL;
}

static struct k_xml_node *k__xml_parse_node(struct k_xml_parser *parser) {

    char *p = parser->p;

    if ('\0' == *p)
        goto err;

    if ('<' == *p) {
        if ('!' == *(p + 1)) {
            struct k_xml_comment_node *comment = k__xml_parse_comment_node(parser);
            if (NULL == comment)
                goto err;
            else
                return &comment->base;
        } else {
            struct k_xml_elem_node *elem= k__xml_parse_elem_node(parser);
            if (NULL == elem)
                goto err;
            else
                return &elem->base;
        }
    }
    else {
        struct k_xml_text_node *text = k__xml_parse_text_node(parser);
        if (NULL == text)
            goto err;
        else
            return &text->base;
    }

err:
    return NULL;
}

static struct k_xml_node *k__xml_parse(char *text) {

    struct k_xml_doc *doc = k__xml_create_doc();
    if (NULL == doc)
        return NULL;

    struct k_xml_parser parser;
    parser.doc = doc;
    parser.p = text;

    while ('\0' != *(parser.p)) {
        struct k_xml_node *node = k__xml_parse_node(&parser);
        if (NULL == node)
            goto err;

        switch (node->type) {
            case K_XML_ELEM_NODE: {
                if (NULL != doc->root)
                    goto err;

                doc->root = node;
                k_list_add_tail(&doc->top_node_list, &node->sibling_link);
                break;
            }
            case K_XML_TEXT_NODE: {
                struct k_xml_text_node *text_node = container_of(node, struct k_xml_text_node, base);
                if ( ! text_node->is_blank) {
                    goto err;
                }

                k_list_add_tail(&doc->top_node_list, &node->sibling_link);
                break;
            }
            case K_XML_COMMENT_NODE: {
                k_list_add_tail(&doc->top_node_list, &node->sibling_link);
                break;
            }
        }
    }

    if (NULL == doc->root)
        goto err;

    return parser.doc->root;

err:
    k__xml_destroy_doc(doc);
    return NULL;
}

/* endregion */

/* region [k_xml] */

struct k_xml_node *k_xml_parse(char *text) {

    if (NULL == text || '\0' == *text)
        return NULL;

    return k__xml_parse(text);
}

void k_xml_free(struct k_xml_node *node) {

    if (NULL == node)
        return;

    k__xml_destroy_doc(node->doc);
}

enum k_xml_node_type k_xml_get_type(struct k_xml_node *node) {
    return node->type;
}

struct k_xml_node *k_xml_get_first_child(struct k_xml_node *elem_node) {

    if (NULL == elem_node)
        return NULL;

    if (K_XML_ELEM_NODE == elem_node->type) {
        struct k_xml_elem_node *elem = container_of(elem_node, struct k_xml_elem_node, base);

        struct k_list *child_list = &elem->child_list;
        if (k_list_is_empty(child_list))
            return NULL;

        struct k_list_node *list_node = k_list_get_first(child_list);
        struct k_xml_node *child = container_of(list_node, struct k_xml_node, sibling_link);

        return child;
    }
    else {
        return NULL;
    }
}

struct k_xml_node *k_xml_get_next_sibling(struct k_xml_node *node) {

    if (NULL == node)
        return NULL;

    struct k_list_node *next = node->sibling_link.next;

    struct k_xml_node *parent = node->parent;
    if (NULL == parent) {
        if (next == k_list_tail(&node->doc->top_node_list))
            return NULL;
    } else {
        struct k_xml_elem_node *elem = container_of(parent, struct k_xml_elem_node, base);
        if (next == k_list_tail(&elem->child_list))
            return NULL;
    }

    struct k_xml_node *sibling = container_of(next, struct k_xml_node, sibling_link);
    return sibling;
}

struct k_xml_node *k_xml_get_prev_sibling(struct k_xml_node *node) {

    if (NULL == node)
        return NULL;

    struct k_list_node *prev = node->sibling_link.prev;

    struct k_xml_node *parent = node->parent;
    if (NULL == parent) {
        if (prev == k_list_head(&node->doc->top_node_list))
            return NULL;
    } else {
        struct k_xml_elem_node *elem = container_of(parent, struct k_xml_elem_node, base);
        if (prev == k_list_head(&elem->child_list))
            return NULL;
    }

    struct k_xml_node *sibling = container_of(prev, struct k_xml_node, sibling_link);
    return sibling;
}

struct k_xml_node *k_xml_get_parent(struct k_xml_node *node) {
    return node->parent;
}

struct k_xml_node *k_xml_find_child_by_tag(struct k_xml_node *node, const char *tag) {

    if (NULL == node)
        return NULL;
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
        struct k_xml_node *child = container_of(iter, struct k_xml_node, sibling_link);

        if (K_XML_ELEM_NODE == child->type) {
            struct k_xml_elem_node *child_elem = container_of(child, struct k_xml_elem_node, base);

            if (0 == strcmp(child_elem->tag, tag))
                return child;
        }
    }

    return NULL;
}

struct k_xml_node *k_xml_find_next_by_tag(struct k_xml_node *node, const char *tag) {

    if (NULL == node)
        return NULL;
    if (NULL == tag || '\0' == *tag)
        return NULL;

    struct k_xml_node *parent = node->parent;
    if (NULL == parent)
        return NULL;

    struct k_xml_elem_node *elem = container_of(parent, struct k_xml_elem_node, base);

    struct k_xml_node *sibling;
    struct k_list *parent_child_list = &elem->child_list;
    struct k_list_node *iter = node->sibling_link.next;
    for (; iter != k_list_tail(parent_child_list); iter = iter->next) {
        sibling = container_of(iter, struct k_xml_node, sibling_link);

        if (K_XML_ELEM_NODE == sibling->type) {
            struct k_xml_elem_node *sibling_elem = container_of(sibling, struct k_xml_elem_node, base);

            if (0 == strcmp(sibling_elem->tag, tag))
                return sibling;
        }
    }

    return NULL;
}

const char *k_xml_get_tag(struct k_xml_node *elem_node) {

    if (NULL == elem_node)
        return NULL;
    if (K_XML_ELEM_NODE != elem_node->type)
        return NULL;

    struct k_xml_elem_node *elem = container_of(elem_node, struct k_xml_elem_node, base);
    return elem->tag;
}

const char *k_xml_get_attr(struct k_xml_node *elem_node, const char *attr_key) {

    if (NULL == elem_node)
        return NULL;
    if (K_XML_ELEM_NODE != elem_node->type)
        return NULL;
    if (NULL == attr_key || '\0' == *attr_key)
        return NULL;

    struct k_xml_elem_node *elem = container_of(elem_node, struct k_xml_elem_node, base);

    struct k_xml_attr *attr;
    struct k_list *attr_list = &elem->attr_list;
    struct k_list_node *iter;
    for (k_list_for_each(attr_list, iter)) {
        attr = container_of(iter, struct k_xml_attr, list_node);

        if (0 == strcmp(attr->key, attr_key))
            return attr->val;
    }

    return NULL;
}

struct k_xml_attr *k_xml_get_first_attr(struct k_xml_node *elem_node, const char **get_key, const char **get_val) {

    if (NULL == elem_node)
        return NULL;
    if (K_XML_ELEM_NODE != elem_node->type)
        return NULL;

    struct k_xml_elem_node *elem = container_of(elem_node, struct k_xml_elem_node, base);

    struct k_list *attr_list = &elem->attr_list;
    if (k_list_is_empty(attr_list))
        return NULL;

    struct k_list_node *first = k_list_get_first(attr_list);
    struct k_xml_attr *attr = container_of(first, struct k_xml_attr, list_node);

    if (NULL != get_key) { *get_key = attr->key; }
    if (NULL != get_val) { *get_val = attr->val; }
    return attr;
}

struct k_xml_attr *k_xml_get_next_attr(struct k_xml_attr *attr, const char **get_key, const char **get_val) {

    if (NULL == attr)
        return NULL;

    struct k_list_node *next = attr->list_node.next;
    if (next == k_list_tail(&attr->elem->attr_list))
        return NULL;

    struct k_xml_attr *next_attr = container_of(next, struct k_xml_attr, list_node);
    if (NULL != get_key) { *get_key = next_attr->key; }
    if (NULL != get_val) { *get_val = next_attr->val; }
    return next_attr;
}

const char *k_xml_get_text(struct k_xml_node *node) {

    if (NULL == node)
        return NULL;

    if (K_XML_TEXT_NODE == node->type) {
        struct k_xml_text_node *text = container_of(node, struct k_xml_text_node, base);
        return text->text;
    }
    else if (K_XML_ELEM_NODE == node->type) {
        struct k_xml_elem_node *elem = container_of(node, struct k_xml_elem_node, base);

        struct k_list *child_list = &elem->child_list;
        if (k_list_is_empty(child_list))
            return NULL;

        struct k_xml_node *child;
        struct k_list_node *iter;
        for (k_list_for_each(child_list, iter)) {
            child = container_of(iter, struct k_xml_node, sibling_link);

            if (K_XML_TEXT_NODE == child->type) {
                struct k_xml_text_node *text = container_of(child, struct k_xml_text_node, base);
                return text->text;
            }
        }

        return "";
    }
    else if (K_XML_COMMENT_NODE == node->type) {
        struct k_xml_comment_node *comment = container_of(node, struct k_xml_comment_node, base);
        return comment->comment;
    }
    else {
        return NULL;
    }
}

/* endregion */
