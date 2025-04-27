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

        int *val4 = k_str_map_put(map, "key4", sizeof(int));
        *val4 = 4;

        int *val5 = k_str_map_put(map, "key5", sizeof(int));
        *val5 = 5;
    }


    struct k_str_map_iter iter;
    const char *key;
    int *val;
    for (k_str_map_for_each(map, &iter, &key, &val)) {

        printf("%s: %d\n", key, *val);
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

