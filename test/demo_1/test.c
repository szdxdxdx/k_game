#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_str_map.h"

static void test(void) {

    struct k_str_map *map = k_str_map_create(NULL);


    struct k_str_map_iter iter;
    const char *key;
    int *val;
    for (k_str_map_for_each(map, &iter, &key, &val)) {

        printf("%s: %d\n", key, *val);
    }
}

#if 0

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    test();

    return 0;
}

#endif

