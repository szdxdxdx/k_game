#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_xml.h"

struct k_xml_token_ident {
    char *begin;
    char *end;
};

struct k_xml_token_string {
    char *begin;
    char *end;
    int entities_num;
};

struct k_xml_token_attr {
    struct k_xml_token_attr *next;
    struct k_xml_token_ident  key;
    struct k_xml_token_string val;
};

struct k_xml_token_node {
    struct k_xml_token_node *parent;
    struct k_xml_token_ident tag;
    struct k_xml_token_attr *attr;
};

struct k_xml_parser {

    char *text;
    char *p;

    struct k_xml_token_node *node;
};

static char *skip_space(char *text) {
    while (*text && isspace((unsigned char)*text)) {
        text++;
    }
    return text;
}

static char *extract_ident(char *text, struct k_xml_token_ident *get_ident) {

    char *begin = skip_space(text);
    if ( ! isalpha((unsigned char)*begin) && '_' != *begin)
        goto err;

    char *end = begin + 1;
    while (isalnum((unsigned char)*end) || '_' == *end) {
        end++;
    }

    get_ident->begin = begin;
    get_ident->end   = end;
    return end;

err:
    return text;
}

static char *extract_string(char *text, struct k_xml_token_string *get_string) {

    char *begin = skip_space(text);
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

    get_string->begin = begin + 1;
    get_string->end   = end;
    get_string->entities_num = entities_count;

    return end + 1;

err:
    return text;
}

static char *extract_attr(char *text, struct k_xml_token_attr *get_attr) {

    struct k_xml_token_ident  key;
    struct k_xml_token_string val;

    char *p = text;
    if (p == extract_ident(p, &key))
        goto err;

    p = skip_space(key.end);
    if ('=' != *p)
        goto err;
    else
        p++;

    if (p == extract_string(p, &val))
        goto err;

    get_attr->key  = key;
    get_attr->val  = val;
    get_attr->next = NULL;
    return 0;

err:
    return text;
}

static int extract_tag_start(struct k_xml_parser *parser) {


}

static int k__xml_parse(struct k_xml_parser *parser) {

    return 0;
}

int k_xml_parse(char *text) {

    struct k_xml_parser parser;
    parser.text = text;
    parser.p    = text;
    k__xml_parse(&parser);

    return 0;
}

static void test_extract_attr(char *text) {
    printf("-------------------------------------------\n");
    printf("attr text: %s\n", text);

    char *p = text;
    struct k_xml_token_attr attr;
    if (p != extract_attr(p, &attr)) {
        printf("attr.key = %.*s\n", (int)(attr.key.end - attr.key.begin), attr.key.begin);
        printf("attr.val = %.*s\n", (int)(attr.val.end - attr.val.begin), attr.val.begin);
    }
    else {
        printf("error\n");
    }
}

int main(void) {

    test_extract_attr(" _=1");
    test_extract_attr("a='hello'");
    test_extract_attr("b  = \"'\n'\"");
    test_extract_attr("a");
    test_extract_attr("0a='hello'");
}
