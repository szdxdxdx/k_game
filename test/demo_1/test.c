#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_str_map.h"

static void test(void) {

    struct k_str_map *map = k_str_map_create(NULL);

    {
        int *val1 = k_str_map_put(map, "key1", sizeof(int));
        *val1 = 1;

        int *val2 = k_str_map_put(map, "key2", sizeof(int));
        *val2 = 2;

        int *val3 = k_str_map_put(map, "key3", sizeof(int));
        *val3 = 3;
    }

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

