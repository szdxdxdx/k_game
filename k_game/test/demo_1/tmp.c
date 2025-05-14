#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#if 0

static void test(void) {

    // 初始化字符串缓冲区，指定默认的缓冲区
    struct k_str_buf str_buf;
    char default_buf[20];
    k_str_buf_init(&str_buf, default_buf, sizeof(default_buf));

    // 往缓冲区中追加写入字符串
    k_str_buf_puts(&str_buf, "hello world!");
    k_str_buf_printf(&str_buf, "%*s", 20, "!");

    // 获取缓冲区中的字符串内容
    char *str = k_str_buf_get(&str_buf);
    printf("%s", str);

    // 结束使用字符串缓冲区
    k_str_buf_free(&str_buf);
}

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    test();

    return 0;
}

#endif

