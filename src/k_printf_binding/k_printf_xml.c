#include <assert.h>

#include "k_printf_binding.h"

#include "k_xml.h"

static void print_xml_node(struct k_printf_buf *buf, struct k_xml_node *node) {

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

    enum k_xml_node_type type = k_xml_get_type(node);
    switch (type) {
        case K_XML_ELEM_NODE: {
            const char *tag = k_xml_get_tag(node);
            k_printf_buf_printf(buf, CYAN "<%s", tag);

            const char *key;
            const char *val;
            struct k_xml_attr *attr = k_xml_get_first_attr(node, &key, &val);
            for (; NULL != attr; attr = k_xml_get_next_attr(attr, &key, &val)) {
                k_printf_buf_printf(buf, " " YELLOW "%s" CYAN "=\"" GREEN "%s" CYAN "\"", key, val);
            }

            struct k_xml_node *child = k_xml_get_first_child(node);

            if (NULL == child) {
                k_printf_buf_printf(buf, CYAN "/>");
                return;
            } else {
                k_printf_buf_printf(buf, CYAN ">");
            }

            for (; NULL != child; child = k_xml_get_next_sibling(child)) {
                print_xml_node(buf, child);
            }

            k_printf_buf_printf(buf, CYAN "</%s>", tag);
            break;
        }
        case K_XML_TEXT_NODE: {
            k_printf_buf_printf(buf, MAGENTA "%s", k_xml_get_text(node));
            break;
        }
        case K_XML_COMMENT_NODE: {
            k_printf_buf_printf(buf, RED "<!--%s-->", k_xml_get_text(node));
            break;
        }
        default:
            assert(0);
    }
}

void k__printf_spec_k_xml(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args) {
    (void)spec;

    struct k_xml_node *node = (struct k_xml_node *)va_arg(*args, void *);

    if (NULL == node) {
        k_printf_buf_printf(buf, "(null)");
    } else {
        print_xml_node(buf, node);
        k_printf_buf_printf(buf, RESET);
    }
}
