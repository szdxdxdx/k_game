#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_dyn_array.h"

static void test(void) {

    struct k_ptr_array *arr = k_ptr_array_create();
    k_ptr_array_push_back(arr, "Hello");
    k_ptr_array_push_back(arr, "World");
    k_ptr_array_push_back(arr, "!");

    int i = 0;
    for (; i < arr->size; i++) {
        const char *s = arr->storage[i];
        printf("%s ", s);
    }

    k_ptr_array_destroy(arr);
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
