#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include "k_xml.h"

static const char *skip_space(const char *p) {
    while (isspace(*p)) {
        p++;
    }
    return p;
}

static int is_ident_start(char ch) {
    return isalpha(ch) || '_' == ch;
}

static int is_ident(char ch) {
    return isalpha(ch) || '_' == ch || ('0' <= ch && ch <= '9');
}

static void tmp(void) {

    char text[] = "<__root><a>1<b>2</b></a><c>3</c></root>";

    k_xml_parse(text);
}

#if 1

int main__(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    tmp();

    return 0;
}

#endif
