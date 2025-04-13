
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "k_xml.h"

#include "k_printf.h"

void k_xml_print(struct k_printf_buf *buf, struct k_xml_node *node) {

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

    enum k_xml_node_type type = k_xml_get_type(node);
    if (K_XML_ELEM_NODE == type) {

        const char *tag = k_xml_get_tag(node);
        buf->fn_printf(buf, CYAN "<%s", tag);

        const char *key;
        const char *val;
        struct k_xml_attr *attr = k_xml_get_first_attr(node, &key, &val);
        for (; NULL != attr; attr = k_xml_get_next_attr(attr, &key, &val)) {
            buf->fn_printf(buf, " " YELLOW "%s" CYAN "=\"" GREEN "%s" CYAN "\"", key, val);
        }

        struct k_xml_node *child = k_xml_get_first_child(node);

        if (NULL == child) {
            buf->fn_printf(buf, CYAN "/>");
            return;
        } else {
            buf->fn_printf(buf, CYAN ">");
        }

        for (; NULL != child; child = k_xml_get_next_sibling(child)) {
            k_xml_print(buf, child);
        }

        buf->fn_printf(buf, CYAN "</%s>", tag);
    }
    else if (K_XML_TEXT_NODE == type) {
        buf->fn_printf(buf, MAGENTA "%s", k_xml_get_text(node));
    }
}

void k_printf_spec_k_xml(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args) {
    (void)spec;

    struct k_xml_node *node = (struct k_xml_node *)va_arg(*args, void *);
    k_xml_print(buf, node);
    buf->fn_printf(buf, RESET);
}

k_printf_callback_fn match_spec_xml(const char **str) {

    if (0 == strncmp(*str, "xml", 3)) {
        *str += 3;
        return k_printf_spec_k_xml;
    } else {
        return NULL;
    }
}

static struct k_printf_config xml = {
    .fn_match_spec = match_spec_xml
};

static void tmp(void) {

    char text[] = "<bookstore a=\"&lt;&lt;\">\n"
                  "    <book title=\"C语言\" author=\"Dennis Ritchie\">\n"
                  "        <summary>这本书讲述了C语言的基本语法和实现原理。</summary>\n"
                  "        <price currency=\"USD\">29.99</price>\n"
                  "        <note>&lt;适合初学者 &amp; 系统程序员</note>\n"
                  "    </book>\n"
                  "    <div/>\n"
                  "    <book title=\"XML解析指南\" author=\"某作者\">\n"
                  "        <summary>内容包括：标签、属性、实体、树结构等。</summary>\n"
                  "        <price currency=\"CNY\">&lt;59.00</price>\n"
                  "    </book>\n"
                  "</bookstore>";

    struct k_xml_node *bookstore = k_xml_parse(text);

    k_printf(&xml, "%xml", bookstore);

    k_xml_free(bookstore);
}

#if 1

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    tmp();

    return 0;
}

#endif
