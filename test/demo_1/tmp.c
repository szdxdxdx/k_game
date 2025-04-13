#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include "k_xml.h"

static void tmp(void) {

    char text[] = "<bookstore>\n"
                  "    <book title=\"C语言\" author=\"Dennis Ritchie\">\n"
                  "        <summary>这本书讲述了C语言的基本语法和实现原理。</summary>\n"
                  "        <price currency=\"USD\">29.99</price>\n"
                  "        <note>适合初学者 &amp; 系统程序员</note>\n"
                  "    </book>\n"
                  "    <div/>"
                  "    <book title=\"XML解析指南\" author=\"某作者\">\n"
                  "        <summary>内容包括：标签、属性、实体、树结构等。</summary>\n"
                  "        <price currency=\"CNY\">59.00</price>\n"
                  "    </book>\n"
                  "</bookstore>";

    struct k_xml_node *root = k_xml_parse(text);

    struct k_xml_node *book_1 = k_xml_find_child_by_tag(root, "book");
    struct k_xml_node *price_1 = k_xml_find_child_by_tag(book_1, "price");

    struct k_xml_node *book_2 = k_xml_find_next_by_tag(book_1, "book");
    struct k_xml_node *price_2 = k_xml_find_child_by_tag(book_2, "price");

    printf("price_1 : %s\n", k_xml_get_text(price_1));
    printf("price_2 : %s\n", k_xml_get_text(price_2));

    k_xml_free(root);
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
