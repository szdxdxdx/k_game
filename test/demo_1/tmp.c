
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_xml.h"

#include "k_printf.h"
#include "k_printf_binding.h"

static void tmp(void) {

    char text[] = "<!-- 注释 -->\n"
                  "<bookstore a=\"&lt;&lt;\">\n"
                  "    <!-- 注释 -->\n"
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

    k_printf(k_fmt, "%k_xml", k_xml_get_prev_sibling(k_xml_get_prev_sibling(bookstore)));
    k_printf(k_fmt, "%k_xml", k_xml_get_prev_sibling(bookstore));
    k_printf(k_fmt, "%k_xml", bookstore);

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
