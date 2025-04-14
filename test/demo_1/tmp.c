
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_xml.h"
#include "k_json.h"

#include "k_printf.h"
#include "k_printf_binding.h"

static void tmp(void) {

    char text[] = "<book id=\"1\" available=\"true\">\n"
                  "     <!-- 测试 -->\n"
                  "     <title>Learn &lt;C&gt;</title>\n"
                  "     price：39.99\n"
                  "</book>";

    struct k_xml_node *book = k_xml_parse(text);

    k_printf(k_fmt, "%k_xml", book);

    k_xml_free(book);
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
