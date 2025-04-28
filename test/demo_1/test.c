#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_str_buf.h"

static void test(void) {

    char default_buf[1024];
    struct k_str_buf buf;
    k_str_buf_init(&buf, default_buf, sizeof(default_buf));

    k_str_buf_printf(&buf, "%*.s!", INT_MAX - 2, "");
    k_str_buf_printf(&buf, "%*.s!", INT_MAX - 2, "");
    k_str_buf_printf(&buf, "%*.s!", INT_MAX - 2, "");

    char *s = k_str_buf_get(&buf);
    if (NULL != s) {
        printf("%s\n", s);
    }
    else {
        printf("error");
    }


    k_str_buf_free(&buf);
}

#if 1

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    test();

    return 0;
}

#endif

