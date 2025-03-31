#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "k_str_map.h"

static void test(void) {

    struct k_str_map_config config;
    config.fn_malloc = malloc;
    config.fn_free   = free;
    struct k_str_map *map = k_str_map_create(&config);
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
