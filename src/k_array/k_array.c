#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include "k_array.h"

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

const static struct k_array_options default_options = {
    .fn_malloc = malloc,
    .fn_free   = free,
};

struct k_array *k_array_create(size_t elem_size, const struct k_array_options *options) {
    assert(0 < elem_size);

    if (NULL == options) {
        options = &default_options;
    }

    assert(NULL != options->fn_malloc);
    assert(NULL != options->fn_free);

    struct k_array *arr = options->fn_malloc(sizeof(struct k_array));
    if (NULL == arr)
        return NULL;

    arr->fn_malloc = options->fn_malloc;
    arr->fn_free   = options->fn_free;
    arr->elem_size = elem_size;
    arr->capacity  = 0;
    arr->size      = 0;
    arr->storage   = NULL;

    return arr;
}

void k_array_destroy(struct k_array *arr) {

    if (NULL == arr)
        return;

    arr->fn_free(arr->storage);
    arr->fn_free(arr);
}

struct k_array *k_array_construct(struct k_array *arr, size_t elem_size, const struct k_array_options *options) {
    assert(NULL != arr);
    assert(0 < elem_size);

    if (NULL == options) {
        options = &default_options;
    }

    assert(NULL != options->fn_malloc);
    assert(NULL != options->fn_free);

    arr->fn_malloc = options->fn_malloc;
    arr->fn_free   = options->fn_free;
    arr->elem_size = elem_size;
    arr->capacity  = 0;
    arr->size      = 0;
    arr->storage   = NULL;

    return arr;
}

void k_array_destruct(struct k_array *arr) {

    if (NULL == arr)
        return;

    arr->fn_free(arr->storage);
}

int k_array_reserve(struct k_array *arr, size_t n) {
    assert(NULL != arr);

    if (n <= arr->capacity)
        return 0;

    if (SIZE_MAX / arr->elem_size <= n)
        return -1;

    size_t new_capacity = arr->capacity * 2;
    if (new_capacity < n)
        new_capacity = n;

    void *new_storage = arr->fn_malloc(new_capacity * arr->elem_size);
    if (NULL == new_storage) {
        if (new_capacity == n) {
            return -1;
        }
        else {
            new_capacity = n;

            new_storage = arr->fn_malloc(new_capacity * arr->elem_size);
            if (NULL == new_storage)
                return -1;
        }
    }

    if (NULL != arr->storage) {
        memcpy(new_storage, arr->storage, arr->size * arr->elem_size);
        arr->fn_free(arr->storage);
    }

    arr->capacity = new_capacity;
    arr->storage  = new_storage;
    return 0;
}

void *k_array_shift_right(struct k_array *arr, size_t idx, size_t n) {
    assert(NULL != arr);
    assert(idx <= arr->size);

    size_t required_capacity = arr->size + n;
    if (required_capacity < n)
        return NULL;

    if (required_capacity <= arr->capacity) {

        void  *shift_from = ptr_offset(arr->storage, idx * arr->elem_size);
        void  *shift_to   = ptr_offset(arr->storage, (idx + n) * arr->elem_size);
        size_t shift_size = (arr->size - idx) * arr->elem_size;
        memmove(shift_to, shift_from, shift_size);

        arr->size += n;
        return shift_from;
    }

    if (SIZE_MAX / arr->elem_size <= required_capacity)
        return NULL;

    size_t new_capacity = arr->capacity * 2;
    if (new_capacity < required_capacity)
        new_capacity = required_capacity;

    void *new_storage = arr->fn_malloc(new_capacity * arr->elem_size);
    if (NULL == new_storage) {
        if (new_capacity == required_capacity) {
            return NULL;
        }
        else {
            new_capacity = required_capacity;

            new_storage = arr->fn_malloc(new_capacity * arr->elem_size);
            if (NULL == new_storage)
                return NULL;
        }
    }

    if (NULL != arr->storage) {
        void  *part_1_from = arr->storage;
        void  *part_1_to   = new_storage;
        size_t part_1_size = idx * arr->elem_size;
        memcpy(part_1_to, part_1_from, part_1_size);

        void  *part_2_from = ptr_offset(arr->storage, part_1_size);
        void  *part_2_to   = ptr_offset(part_1_to, n * arr->elem_size);
        size_t part_2_size = (arr->size - idx) * arr->elem_size;
        memcpy(part_2_to, part_2_from, part_2_size);

        arr->fn_free(arr->storage);
    }

    arr->capacity = new_capacity;
    arr->size    += n;
    arr->storage  = new_storage;
    return ptr_offset(arr->storage, idx * arr->elem_size);
}

int k_array_insert_all(struct k_array *arr, size_t idx, const void *elems, size_t elems_num) {
    assert(NULL != arr);
    assert(idx <= arr->size);
    assert(NULL != elems);

    /* 要添加的元素不能来自容器内部，即 `elems` 所指向的内存段不能和数组的 `storage` 所指向的内存段有交集。
     * 若有交集，在执行插入操作时，数组会腾挪元素或动态扩容，导致 `elems` 所指向内存段的数据会发生变化或失效。
     *
     * 当然，可以先判断 `elems` 是否在容器内，若是，则分配内存复制元素后再执行插入操作，
     * 但这会导致 `elems` 指向的内存段的数据发生改变，与 const 的语义不符。
     */
    assert( ! (arr->storage <= elems && elems < ptr_offset(arr->storage, arr->capacity * arr->elem_size)));

    void *insert_position = k_array_shift_right(arr, idx, elems_num);
    if (NULL == insert_position)
        return -1;

    memcpy(insert_position, elems, elems_num * arr->elem_size);
    return 0;
}

int k_array_insert(struct k_array *arr, size_t idx, const void *elem) {
    return k_array_insert_all(arr, idx, elem, 1);
}

int k_array_push_back_all(struct k_array *arr, const void *elems, size_t elems_num) {
    return k_array_insert_all(arr, arr->size, elems, elems_num);
}

int k_array_push_back(struct k_array *arr, const void *elem) {
    return k_array_insert_all(arr, arr->size, elem, 1);
}

void k_array_shift_left(struct k_array *arr, size_t idx, size_t n) {
    assert(NULL != arr);
    assert(idx <= arr->size);
    assert(n <= idx);

    void  *shift_from = ptr_offset(arr->storage, idx * arr->elem_size);
    void  *shift_to   = ptr_offset(arr->storage, (idx - n) * arr->elem_size);
    size_t shift_size = (arr->size - idx) * arr->elem_size;
    memmove(shift_to, shift_from, shift_size);

    arr->size -= n;
}

void k_array_remove_range(struct k_array *arr, size_t idx_from, size_t idx_to) {
    k_array_shift_left(arr, idx_to, idx_to - idx_from);
}

void k_array_remove(struct k_array *arr, size_t idx) {
    k_array_shift_left(arr, idx + 1, 1);
}

void k_array_pop_back(struct k_array *arr) {
    assert(NULL != arr);
    assert(0 != arr->size);

    arr->size -= 1;
}

void k_array_clear(struct k_array *arr) {
    assert(NULL != arr);

    arr->size = 0;
}

void k_array_free_storage(struct k_array *arr) {
    assert(NULL != arr);

    arr->fn_free(arr->storage);

    arr->capacity = 0;
    arr->size     = 0;
    arr->storage  = NULL;
}
