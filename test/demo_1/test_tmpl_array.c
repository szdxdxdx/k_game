#include <stdlib.h>
#include <stdio.h>

#include "./k_int_array.h"
#include "./k_array.h"

static void test(void) {

    struct k_int_array *arr = k_int_array_create(8);

    int elem = 1;
    k_int_array_push_back(arr, elem);

    int elems[] = { 1, 4, 5, 1, 4 };
    k_int_array_push_back_all(arr, elems, 5);

    int i = 0;
    for (; i < arr->size; i++) {
        printf("%d ", arr->storage[i]);
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
