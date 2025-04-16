#include <stdlib.h>
#include <stdio.h>

#define k_tmpl_array_elem_type float
#define k_tmpl_array_name      float_array
#define k_tmpl_array_pass_elem_by_val
#define k_tmpl_array_impl
#include "k_tmpl_array.h"

static void test(void) {

    struct float_array *arr = float_array_create(8);

    float_array_push_back(arr, 1);

    float elems[] = { 1, 4, 5, 1, 4 };
    float_array_push_back_all(arr, elems, 5);

    int i = 0;
    for (; i < arr->size; i++) {
        printf("%.0f ", arr->storage[i]);
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
