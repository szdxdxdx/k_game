#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#if 1

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    printf("hello world");
    return 0;
}

#endif

