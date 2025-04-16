
#if defined(k_tmpl_array_name) && defined(k_tmpl_array_elem_type)

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

/* region [internal] */

/* region [fn_malloc] */

#if defined(k_tmpl_array_fn_malloc) && defined(k_tmpl_array_fn_free)
#define k__tmpl_array_mem_alloc k_tmpl_array_fn_malloc
#define k__tmpl_array_mem_free  k_tmpl_array_fn_free
#elif defined(k_tmpl_array_fn_malloc) || defined(k_tmpl_array_fn_free)
/* 必须同时指明内存分配函数和释放函数 */
#error "Must define both `k__tmpl_array_fn_malloc` and `k__tmpl_array_fn_free`"
#else
#define k__tmpl_array_mem_alloc malloc
#define k__tmpl_array_mem_free  free
#endif

/* endregion */

/* region [pass_elem] */

#ifdef k_tmpl_array_pass_elem_by_val
#define k__tmpl_array_pass_elem_by_val 1
#else
#define k__tmpl_array_pass_elem_by_val 0
#endif

/* endregion */

/* region [static] */

#ifdef k_tmpl_array_static_fn
#define k__tmpl_array_static_fn static
#else
#define k__tmpl_array_static_fn
#endif

/* endregion */

#define k__tmpl_array k_tmpl_array_name

#define k__tmpl_array_x_(array_name, x) array_name##_##x
#define k__tmpl_array_x(array_name, x) k__tmpl_array_x_(array_name, x)
#define k__tmpl_array_(x) k__tmpl_array_x(k_tmpl_array_name, x)

/* endregion */

struct k__tmpl_array {

    size_t capacity;

    size_t size;

    k_tmpl_array_elem_type *storage;
};

k__tmpl_array_static_fn struct k__tmpl_array *k__tmpl_array_(create)(size_t init_capacity);

k__tmpl_array_static_fn void k__tmpl_array_(destroy)(struct k__tmpl_array *arr);

k__tmpl_array_static_fn struct k__tmpl_array *k__tmpl_array_(construct)(struct k__tmpl_array *arr, size_t init_capacity);

k__tmpl_array_static_fn void k__tmpl_array_(destruct)(struct k__tmpl_array *arr);

k__tmpl_array_static_fn int k__tmpl_array_(reserve)(struct k__tmpl_array *arr, size_t n);

k__tmpl_array_static_fn void *k__tmpl_array_(shift_right)(struct k__tmpl_array *arr, size_t idx, size_t n);

k__tmpl_array_static_fn int k__tmpl_array_(insert_all)(struct k__tmpl_array *arr, size_t idx, k_tmpl_array_elem_type *elems, size_t elems_num);

k__tmpl_array_static_fn int k__tmpl_array_(push_back_all)(struct k__tmpl_array *arr, k_tmpl_array_elem_type *elems, size_t elems_num);

#if k__tmpl_array_pass_elem_by_val

k__tmpl_array_static_fn int k__tmpl_array_(insert)(struct k__tmpl_array *arr, size_t idx, k_tmpl_array_elem_type elem);

k__tmpl_array_static_fn int k__tmpl_array_(push_back)(struct k__tmpl_array *arr, k_tmpl_array_elem_type elem);

#else

k__tmpl_array_static_fn int k__tmpl_array_(insert)(struct k__tmpl_array *arr, size_t idx, k_tmpl_array_elem_type *elem);

k__tmpl_array_static_fn int k__tmpl_array_(push_back)(struct k__tmpl_array *arr, k_tmpl_array_elem_type *elem);

#endif

k__tmpl_array_static_fn void k__tmpl_array_(shift_left)(struct k__tmpl_array *arr, size_t idx, size_t n);

k__tmpl_array_static_fn void k__tmpl_array_(remove_range)(struct k__tmpl_array *arr, size_t idx_from, size_t idx_to);

k__tmpl_array_static_fn void k__tmpl_array_(remove)(struct k__tmpl_array *arr, size_t idx);

k__tmpl_array_static_fn void k__tmpl_array_(pop_back)(struct k__tmpl_array *arr);

k__tmpl_array_static_fn void k__tmpl_array_(clear)(struct k__tmpl_array *arr);

k__tmpl_array_static_fn void k__tmpl_array_(free_storage)(struct k__tmpl_array *arr);

k__tmpl_array_static_fn void k__tmpl_array_(fill)(struct k__tmpl_array *arr, size_t idx_from, size_t idx_to, k_tmpl_array_elem_type *elem);

#if defined(k_tmpl_array_impl)

k__tmpl_array_static_fn struct k__tmpl_array *k__tmpl_array_(create)(size_t init_capacity) {

    struct k__tmpl_array *arr = k__tmpl_array_mem_alloc(sizeof(struct k__tmpl_array));
    if (NULL == arr)
        return NULL;

    arr->capacity  = 0;
    arr->size      = 0;
    arr->storage   = NULL;

    if (0 != k__tmpl_array_(reserve)(arr, init_capacity)) {
        k__tmpl_array_mem_free(arr);
        return NULL;
    }

    return arr;
}

k__tmpl_array_static_fn void k__tmpl_array_(destroy)(struct k__tmpl_array *arr) {

    if (NULL == arr)
        return;

    k__tmpl_array_mem_free(arr->storage);
    k__tmpl_array_mem_free(arr);
}

k__tmpl_array_static_fn struct k__tmpl_array *k__tmpl_array_(construct)(struct k__tmpl_array *arr, size_t init_capacity) {
    assert(NULL != arr);

    arr->capacity  = 0;
    arr->size      = 0;
    arr->storage   = NULL;

    if (0 != k__tmpl_array_(reserve)(arr, init_capacity))
        return NULL;

    return arr;
}

k__tmpl_array_static_fn void k__tmpl_array_(destruct)(struct k__tmpl_array *arr) {

    if (NULL == arr)
        return;

    k__tmpl_array_mem_free(arr->storage);
}

k__tmpl_array_static_fn int k__tmpl_array_(reserve)(struct k__tmpl_array *arr, size_t n) {
    assert(NULL != arr);

    if (n <= arr->capacity)
        return 0;

    if (SIZE_MAX / sizeof(k_tmpl_array_elem_type) <= n)
        return -1;

    size_t new_capacity = arr->capacity * 2;
    if (new_capacity < n)
        new_capacity = n;

    void *new_storage = k__tmpl_array_mem_alloc(new_capacity * sizeof(k_tmpl_array_elem_type));
    if (NULL == new_storage) {
        if (new_capacity == n) {
            return -1;
        }
        else {
            new_capacity = n;

            new_storage = k__tmpl_array_mem_alloc(new_capacity * sizeof(k_tmpl_array_elem_type));
            if (NULL == new_storage)
                return -1;
        }
    }

    memcpy(new_storage, arr->storage, arr->size * sizeof(k_tmpl_array_elem_type));
    k__tmpl_array_mem_free(arr->storage);

    arr->capacity = new_capacity;
    arr->storage  = new_storage;
    return 0;
}

k__tmpl_array_static_fn void *k__tmpl_array_(shift_right)(struct k__tmpl_array *arr, size_t idx, size_t n) {
    assert(NULL != arr);
    assert(idx <= arr->size);

    size_t required_capacity = arr->size + n;
    if (required_capacity < n)
        return NULL;

    if (required_capacity <= arr->capacity) {

        void  *shift_from = &arr->storage[idx];
        void  *shift_to   = &arr->storage[idx + n];
        size_t shift_size = (arr->size - idx) * sizeof(k_tmpl_array_elem_type);
        memmove(shift_to, shift_from, shift_size);

        arr->size += n;
        return shift_from;
    }

    if (SIZE_MAX / sizeof(k_tmpl_array_elem_type) <= required_capacity)
        return NULL;

    size_t new_capacity = arr->capacity * 2;
    if (new_capacity < required_capacity)
        new_capacity = required_capacity;

    void *new_storage = k__tmpl_array_mem_alloc(new_capacity * sizeof(k_tmpl_array_elem_type));
    if (NULL == new_storage) {
        if (new_capacity == required_capacity) {
            return NULL;
        }
        else {
            new_capacity = required_capacity;

            new_storage = k__tmpl_array_mem_alloc(new_capacity * sizeof(k_tmpl_array_elem_type));
            if (NULL == new_storage)
                return NULL;
        }
    }

    if (NULL != arr->storage) {
        void  *part_1_from = &arr->storage[0];
        void  *part_1_to   = &new_storage[0];
        size_t part_1_size = idx * sizeof(k_tmpl_array_elem_type);
        memcpy(part_1_to, part_1_from, part_1_size);

        void  *part_2_from = &arr->storage[part_1_size];
        void  *part_2_to   = &new_storage[n];
        size_t part_2_size = (arr->size - idx) * sizeof(k_tmpl_array_elem_type);
        memcpy(part_2_to, part_2_from, part_2_size);

        k__tmpl_array_mem_free(arr->storage);
    }

    arr->capacity = new_capacity;
    arr->size    += n;
    arr->storage  = new_storage;
    return &arr->storage[idx];
}

k__tmpl_array_static_fn int k__tmpl_array_(insert_all)(struct k__tmpl_array *arr, size_t idx, k_tmpl_array_elem_type *elems, size_t elems_num) {
    assert(NULL != arr);
    assert(idx <= arr->size);
    assert(NULL != elems);

    void *insert_position = k__tmpl_array_(shift_right)(arr, idx, elems_num);
    if (NULL == insert_position)
        return -1;

    memcpy(insert_position, elems, elems_num * sizeof(k_tmpl_array_elem_type));
    return 0;
}

k__tmpl_array_static_fn int k__tmpl_array_(push_back_all)(struct k__tmpl_array *arr, k_tmpl_array_elem_type *elems, size_t elems_num) {
    return k__tmpl_array_(insert_all)(arr, arr->size, elems, elems_num);
}

#if k__tmpl_array_pass_elem_by_val

k__tmpl_array_static_fn int k__tmpl_array_(insert)(struct k__tmpl_array *arr, size_t idx, k_tmpl_array_elem_type elem) {
    return k__tmpl_array_(insert_all)(arr, idx, &elem, 1);
}

k__tmpl_array_static_fn int k__tmpl_array_(push_back)(struct k__tmpl_array *arr, k_tmpl_array_elem_type elem) {
    return k__tmpl_array_(insert_all)(arr, arr->size, &elem, 1);
}

#else

k__tmpl_array_static_fn int k__tmpl_array_(insert)(struct k__tmpl_array *arr, size_t idx, k_tmpl_array_elem_type *elem) {
    return k__tmpl_array_(insert_all)(arr, idx, elem, 1);
}

k__tmpl_array_static_fn int k__tmpl_array_(push_back)(struct k__tmpl_array *arr, k_tmpl_array_elem_type *elem) {
    return k__tmpl_array_(insert_all)(arr, arr->size, elem, 1);
}

#endif

k__tmpl_array_static_fn void k__tmpl_array_(shift_left)(struct k__tmpl_array *arr, size_t idx, size_t n) {
    assert(NULL != arr);
    assert(idx <= arr->size);
    assert(n <= idx);

    void  *shift_from = &arr->storage[idx];
    void  *shift_to   = &arr->storage[idx - n];
    size_t shift_size = (arr->size - idx) * sizeof(k_tmpl_array_elem_type);
    memmove(shift_to, shift_from, shift_size);

    arr->size -= n;
}

k__tmpl_array_static_fn void k__tmpl_array_(remove_range)(struct k__tmpl_array *arr, size_t idx_from, size_t idx_to) {
    k__tmpl_array_(shift_left)(arr, idx_to, idx_to - idx_from);
}

k__tmpl_array_static_fn void k__tmpl_array_(remove)(struct k__tmpl_array *arr, size_t idx) {
    k__tmpl_array_(shift_left)(arr, idx + 1, 1);
}

k__tmpl_array_static_fn void k__tmpl_array_(pop_back)(struct k__tmpl_array *arr) {
    assert(NULL != arr);
    assert(0 != arr->size);

    arr->size -= 1;
}

k__tmpl_array_static_fn void k__tmpl_array_(clear)(struct k__tmpl_array *arr) {
    assert(NULL != arr);

    arr->size = 0;
}

k__tmpl_array_static_fn void k__tmpl_array_(free_storage)(struct k__tmpl_array *arr) {
    assert(NULL != arr);

    k__tmpl_array_mem_free(arr->storage);

    arr->capacity = 0;
    arr->size     = 0;
    arr->storage  = NULL;
}

k__tmpl_array_static_fn void k__tmpl_array_(fill)(struct k__tmpl_array *arr, size_t idx_from, size_t idx_to, k_tmpl_array_elem_type *elem) {
    assert(NULL != arr);
    assert(idx_from <= idx_to);
    assert(idx_to <= arr->size);
    assert(NULL != elem);

    void *p_from = &arr->storage[idx_from];
    void *p_to   = &arr->storage[idx_to];

    memcpy(p_from, elem, sizeof(k_tmpl_array_elem_type));

    void *p_now = (void *)((uintptr_t)p_from + sizeof(k_tmpl_array_elem_type));
    for (;;) {
        size_t filled_size    = (uintptr_t)p_now - (uintptr_t)p_from;
        size_t remaining_size = (uintptr_t)p_to  - (uintptr_t)p_now;

        if (filled_size < remaining_size) {
            memcpy(p_now, p_from, filled_size);
            p_now = (void *)((uintptr_t)p_now + filled_size);
        }
        else {
            memcpy(p_now, p_from, remaining_size);
            break;
        }
    }
}

#undef k__tmpl_array_impl

#endif

#undef k__tmpl_array_
#undef k__tmpl_array_x_impl_
#undef k__tmpl_array_x_impl
#undef k__tmpl_array_mem_alloc
#undef k__tmpl_array_mem_free
#undef k__tmpl_array_pass_elem_by_val

#undef k_tmpl_array_name
#undef k_tmpl_array_elem_type
#undef k_tmpl_array_fn_malloc
#undef k_tmpl_array_fn_free
#undef k_tmpl_array_pass_elem_by_val

#endif
