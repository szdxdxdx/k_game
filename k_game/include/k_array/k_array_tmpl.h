
/* 本头文件为动态数组的宏模板，支持以下参数：
 *
 * - K_TMPL_ARRAY_STRUCT_NAME      必须定义此宏，用于指定数组结构体的名字
 * - K_TMPL_ARRAY_ELEM_TYPE        必须定义此宏，指定数组中元素的类型
 * - K_TMPL_ARRAY_DEFINE_STRUCT    若定义此宏，则生成结构体定义，否则只声明
 * - K_TMPL_ARRAY_DEFINE_FUNCTION  若定义此宏，则生成函数定义，否则只声明
 * - K_TMPL_ARRAY_FN_MALLOC        若定义此宏，则使用指定的内存分配函数，否则使用 `malloc()`
 * - K_TMPL_ARRAY_FN_FREE          若定义此宏，则使用指定的内存释放函数，否则使用 `free()`
 * - K_TMPL_ARRAY_PASS_ELEM_BY_PTR 若定义此宏，则操作单个元素（例如：插入单个元素）时传递指针，否则传值
 * - K_TMPL_ARRAY_STATIC_FUNCTION  若定义此宏，则用 static 修饰函数
 *
 *
 * 本头文件的使用方式如下：
 * ```C
 * // my_int_array.h
 * #define K_TMPL_ARRAY_STRUCT_NAME my_int_array // 定义数组结构体名字
 * #define K_TMPL_ARRAY_ELEM_TYPE   int          // 指定数组中元素的类型
 * #define K_TMPL_ARRAY_DEFINE_STRUCT            // 生成结构体定义
 * #include "k_array_tmpl.h"                     // 引入头文件，生成代码
 *
 * // my_int_array.c
 * #define K_TMPL_ARRAY_STRUCT_NAME my_int_array // 定义数组结构体名字，与 my_int_array.h 中指定的名字保持一致
 * #define K_TMPL_ARRAY_ELEM_TYPE   int          // 指定数组中元素的类型
 * #define K_TMPL_ARRAY_DEFINE_FUNCTION          // 生成函数定义
 * #include "./k_array_tmpl.h"                   // 引入头文件，生成代码
 * ```
 */

#if ! defined(K_TMPL_ARRAY_STRUCT_NAME) || ! defined(K_TMPL_ARRAY_ELEM_TYPE)
#error "Must define `K_TMPL_ARRAY_STRUCT_NAME` and `K_TMPL_ARRAY_ELEM_TYPE` before including k_array_tmpl.h"
#else

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>

/* region [malloc] */

#if defined(K_TMPL_ARRAY_FN_MALLOC) && defined(K_TMPL_ARRAY_FN_FREE)
#define k__tmpl_array_fn_malloc K_TMPL_ARRAY_FN_MALLOC
#define k__tmpl_array_fn_free   K_TMPL_ARRAY_FN_FREE
#elif defined(K_TMPL_ARRAY_FN_MALLOC) || defined(K_TMPL_ARRAY_FN_FREE)
/* 必须同时指明内存分配函数和释放函数 */
#error "Must set define `K_TMPL_ARRAY_FN_MALLOC` and `K_TMPL_ARRAY_FN_FREE`"
#else
#include <stdlib.h>
#define k__tmpl_array_fn_malloc malloc
#define k__tmpl_array_fn_free   free
#endif

/* endregion */

/* region [static fn] */

#ifdef k_tmpl_array_static_fn
#define k__tmpl_array_fn static
#else
#define k__tmpl_array_fn
#endif

/* endregion */

/* region */

/* 数组结构体的名字，和数组中元素的类型 */
#define k__tmpl_array      K_TMPL_ARRAY_STRUCT_NAME
#define k__tmpl_array_elem K_TMPL_ARRAY_ELEM_TYPE

/* 用于将结构体名与其他标识符拼接 */
#define k__tmpl_array_x_(array_name, x) array_name##_##x
#define k__tmpl_array_x(array_name, x)  k__tmpl_array_x_(array_name, x)
#define k__tmpl_array_(x)               k__tmpl_array_x(K_TMPL_ARRAY_STRUCT_NAME, x)

/* endregion */

/* region [def_struct] */

/* 生成结构体声明 */
struct k__tmpl_array;

/* 生成结构体定义 */
#if defined(K_TMPL_ARRAY_DEFINE_STRUCT)

/** \brief 支持动态扩容的数组容器 */
struct k__tmpl_array {

    /** \brief [read-only] 数组容量，即在不重新分配存储的情况下最多能容纳的元素数量 */
    size_t capacity;

    /** \brief [read-only] 容器持有的元素的数量 */
    size_t size;

    /** \brief [read-only] 指向存储元素的连续空间的指针 */
    k__tmpl_array_elem *storage;
};

#endif

/* endregion */

/* region [function] */

/* 生成函数声明 */

/**
 * \brief 创建数组
 *
 * 若创建成功，函数返回数组容器的指针，否则返回 `NULL`。
 */
k__tmpl_array_fn struct k__tmpl_array *k__tmpl_array_(create)(void);

/**
 * \brief 销毁数组
 *
 * 若 `arr` 为 `NULL`，则函数立即返回。
 */
k__tmpl_array_fn void k__tmpl_array_(destroy)(struct k__tmpl_array *arr);

/**
 * \brief 构造数组
 *
 * 在 `arr` 所指向的内存段上原地构造数组。
 *
 * 若构造成功，函数返回值同入参 `arr`，否则返回 `NULL`。
 */
k__tmpl_array_fn struct k__tmpl_array *k__tmpl_array_(construct)(struct k__tmpl_array *arr);

/**
 * \brief 析构数组
 *
 * 原地析构 `arr` 所指向的内存段上的动态数组。
 * 若 `arr` 为 `NULL`，则函数立即返回。
 */
k__tmpl_array_fn void k__tmpl_array_(destruct)(struct k__tmpl_array *arr);

/**
 * \brief 调整数组容量的到不小于指定值
 *
 * 若需多次向数组中增加元素，可使用此函数提前分配好容量，以减少扩容次数。
 *
 * 指定预期容量 `n`，函数将调整数组容量到大于或等于 `n`。
 * 若 `n` 小于当前容量，则函数立即返回。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
k__tmpl_array_fn int k__tmpl_array_(reserve)(struct k__tmpl_array *arr, size_t n);

/**
 * \brief 将指定索引及其往后的所有元素右移，为插入操作腾出空间
 *
 * 往数组中插入新元素分两步：
 * 第一步，移动插入点往后的元素，为新元素腾出空间。
 * 第二步，在插入点覆盖写入新元素。
 *
 * 本函数实现的是第一步。
 * 函数将数组中索引 `idx` 往后的所有元素往尾部方向移动，在 `idx` 处腾出 `n` 个元素的空间。
 * 特殊地，若 `idx` 等于数组的 `size`，则等价于在数组末尾开辟一段空间。
 *
 * 若成功，数组 `size` 增加 `n`，函数返回所腾出空间的首地址。
 * 该地址在执行下一次增删或扩缩容操作前始终有效，新元素在该处开始往后覆盖写入。
 *
 * 若失败，数组 `size` 不变，函数返回 `NULL`。
 *
 * 若数组 `arr` 存储的元素类型为 `T`，数组结构体名字为 `k_tmpl_array`，
 * 要在索引 `idx` 处增加 `n` 个元素，可以这样操作：
 * ```C
 * T *insert_pos = k_tmpl_array_shift_right(arr, idx, n);
 * if (NULL != insert_pos) {
 *     insert_pos[0]   = ...;
 *     insert_pos[1]   = ...;
 *     ...
 *     insert_pos[n-1] = ...;
 * }
 * ```
 */
k__tmpl_array_fn k__tmpl_array_elem *k__tmpl_array_(shift_right)(struct k__tmpl_array *arr, size_t idx, size_t n);

/**
 * \brief 在数组指定位置插入多个元素
 *
 * 本函数将 `elems` 指向的内存段中的 `elems_num` 个元素复制插入到数组 `arr` 中索引为 `idx` 的位置，
 * 若成功，函数返回 0，否则返回非 0。
 */
k__tmpl_array_fn int k__tmpl_array_(insert_all)(struct k__tmpl_array *arr, size_t idx, k__tmpl_array_elem *elems, size_t elems_num);

#if ! defined(K_TMPL_ARRAY_PASS_ELEM_BY_PTR)

/** \brief 在数组指定位置插入一个元素。若成功，函数返回 0，否则返回非 0 */
k__tmpl_array_fn int k__tmpl_array_(insert)(struct k__tmpl_array *arr, size_t idx, k__tmpl_array_elem elem);

#else

/** \brief 在数组指定位置插入一个元素，若成功，函数返回 0，否则返回非 0 */
k__tmpl_array_fn int k__tmpl_array_(insert)(struct k__tmpl_array *arr, size_t idx, const k__tmpl_array_elem *elem);

#endif

/** \brief 在数组尾部追加多个元素。若成功，函数返回 0，否则返回非 0 */
k__tmpl_array_fn int k__tmpl_array_(push_back_all)(struct k__tmpl_array *arr, k__tmpl_array_elem *elems, size_t elems_num);

#if ! defined(K_TMPL_ARRAY_PASS_ELEM_BY_PTR)

/** \brief 在数组尾部追加一个元素。若成功，函数返回 0，否则返回非 0 */
k__tmpl_array_fn int k__tmpl_array_(push_back)(struct k__tmpl_array *arr, k__tmpl_array_elem elem);

#else

/** \brief 在数组尾部追加一个元素，若成功，函数返回 0，否则返回非 0 */
k__tmpl_array_fn int k__tmpl_array_(push_back)(struct k__tmpl_array *arr, const k__tmpl_array_elem *elem);

#endif

/**
 * \brief 将数组指定索引后的元素左移，覆盖删除前面的元素
 *
 * 将数组中索引 `idx` 及其往后的所有元素往数组头部方向移动 `n` 个位置，
 * 覆盖删除 `idx` 往左的 `n` 个元素。
 *
 * 调用本函数后，数组 `size` 减少 `n`。
 */
k__tmpl_array_fn void k__tmpl_array_(shift_left)(struct k__tmpl_array *arr, size_t idx, size_t n);

/** \brief 删除数组中指定区间（左闭右开）内的元素 */
k__tmpl_array_fn void k__tmpl_array_(remove_range)(struct k__tmpl_array *arr, size_t idx_from, size_t idx_to);

/** \brief 删除数组中指定索引处的元素 */
k__tmpl_array_fn void k__tmpl_array_(remove)(struct k__tmpl_array *arr, size_t idx);

/** \brief 删除数组最末尾的元素 */
k__tmpl_array_fn void k__tmpl_array_(pop_back)(struct k__tmpl_array *arr);

/** \brief 清空数组，但保持原容量，不释放存储元素所使用的内存 */
k__tmpl_array_fn void k__tmpl_array_(clear)(struct k__tmpl_array *arr);

/** \brief 清空数组，并释放存储元素所用的内存 */
k__tmpl_array_fn void k__tmpl_array_(free_storage)(struct k__tmpl_array *arr);

/* endregion */

/* region [function] */

/* 生成函数实现 */
#if defined(K_TMPL_ARRAY_DEFINE_FUNCTION)

k__tmpl_array_fn struct k__tmpl_array *k__tmpl_array_(create)(void) {

    struct k__tmpl_array *arr = k__tmpl_array_fn_malloc(sizeof(struct k__tmpl_array));
    if (NULL == arr)
        return NULL;

    arr->capacity  = 0;
    arr->size      = 0;
    arr->storage   = NULL;

    return arr;
}

k__tmpl_array_fn void k__tmpl_array_(destroy)(struct k__tmpl_array *arr) {

    if (NULL == arr)
        return;

    k__tmpl_array_fn_free(arr->storage);
    k__tmpl_array_fn_free(arr);
}

k__tmpl_array_fn struct k__tmpl_array *k__tmpl_array_(construct)(struct k__tmpl_array *arr) {
    assert(NULL != arr);

    arr->capacity  = 0;
    arr->size      = 0;
    arr->storage   = NULL;

    return arr;
}

k__tmpl_array_fn void k__tmpl_array_(destruct)(struct k__tmpl_array *arr) {

    if (NULL == arr)
        return;

    k__tmpl_array_fn_free(arr->storage);
}

k__tmpl_array_fn int k__tmpl_array_(reserve)(struct k__tmpl_array *arr, size_t n) {
    assert(NULL != arr);

    if (n <= arr->capacity)
        return 0;

    if (SIZE_MAX / sizeof(k__tmpl_array_elem) <= n)
        return -1;

    size_t new_capacity = arr->capacity * 2;
    if (new_capacity < n)
        new_capacity = n;

    k__tmpl_array_elem *new_storage = k__tmpl_array_fn_malloc(new_capacity * sizeof(k__tmpl_array_elem));
    if (NULL == new_storage) {
        if (new_capacity == n) {
            return -1;
        }
        else {
            new_capacity = n;

            new_storage = k__tmpl_array_fn_malloc(new_capacity * sizeof(k__tmpl_array_elem));
            if (NULL == new_storage)
                return -1;
        }
    }

    memcpy(new_storage, arr->storage, arr->size * sizeof(k__tmpl_array_elem));
    k__tmpl_array_fn_free(arr->storage);

    arr->capacity = new_capacity;
    arr->storage  = new_storage;
    return 0;
}

k__tmpl_array_fn k__tmpl_array_elem *k__tmpl_array_(shift_right)(struct k__tmpl_array *arr, size_t idx, size_t n) {
    assert(NULL != arr);
    assert(idx <= arr->size);

    size_t required_capacity = arr->size + n;
    if (required_capacity < n)
        return NULL;

    if (required_capacity <= arr->capacity) {

        k__tmpl_array_elem *shift_from = &arr->storage[idx];
        k__tmpl_array_elem *shift_to   = &arr->storage[idx + n];
        size_t shift_size = (arr->size - idx) * sizeof(k__tmpl_array_elem);
        memmove(shift_to, shift_from, shift_size);

        arr->size += n;
        return shift_from;
    }

    if (SIZE_MAX / sizeof(k__tmpl_array_elem) <= required_capacity)
        return NULL;

    size_t new_capacity = arr->capacity * 2;
    if (new_capacity < required_capacity)
        new_capacity = required_capacity;

    k__tmpl_array_elem *new_storage = k__tmpl_array_fn_malloc(new_capacity * sizeof(k__tmpl_array_elem));
    if (NULL == new_storage) {
        if (new_capacity == required_capacity) {
            return NULL;
        }
        else {
            new_capacity = required_capacity;

            new_storage = k__tmpl_array_fn_malloc(new_capacity * sizeof(k__tmpl_array_elem));
            if (NULL == new_storage)
                return NULL;
        }
    }

    if (NULL != arr->storage) {
        k__tmpl_array_elem *part_1_from = &arr->storage[0];
        k__tmpl_array_elem *part_1_to   = &new_storage[0];
        size_t part_1_size = idx * sizeof(k__tmpl_array_elem);
        memcpy(part_1_to, part_1_from, part_1_size);

        k__tmpl_array_elem *part_2_from = &arr->storage[part_1_size];
        k__tmpl_array_elem *part_2_to   = &new_storage[n];
        size_t part_2_size = (arr->size - idx) * sizeof(k__tmpl_array_elem);
        memcpy(part_2_to, part_2_from, part_2_size);

        k__tmpl_array_fn_free(arr->storage);
    }

    arr->capacity = new_capacity;
    arr->size    += n;
    arr->storage  = new_storage;
    return &arr->storage[idx];
}

k__tmpl_array_fn int k__tmpl_array_(insert_all)(struct k__tmpl_array *arr, size_t idx, k__tmpl_array_elem *elems, size_t elems_num) {
    assert(NULL != arr);
    assert(idx <= arr->size);
    assert(NULL != elems);

    /* 要添加的元素不能来自容器内部，即 `elems` 所指向的内存段不能和数组的 `storage` 所指向的内存段有交集。
     * 若有交集，在执行插入操作时，数组会腾挪元素或动态扩容，导致 `elems` 所指向内存段的数据会发生变化或失效。
     *
     * 或者，我可以先判断 `elems` 是否在容器内，若是则分配内存复制一份元素，再执行插入操作。
     * 但操作结束后， `elems` 指向的内存段的数据可能发生了改变，与 const 的语义不符。
     */
    assert( ! (arr->storage <= elems && elems < &arr->storage[arr->capacity]));

    void *insert_position = k__tmpl_array_(shift_right)(arr, idx, elems_num);
    if (NULL == insert_position)
        return -1;

    memcpy(insert_position, elems, elems_num * sizeof(k__tmpl_array_elem));
    return 0;
}

#if ! defined(K_TMPL_ARRAY_PASS_ELEM_BY_PTR)

k__tmpl_array_fn int k__tmpl_array_(insert)(struct k__tmpl_array *arr, size_t idx, k__tmpl_array_elem elem) {
    return k__tmpl_array_(insert_all)(arr, idx, &elem, 1);
}

#else

k__tmpl_array_fn int k__tmpl_array_(insert)(struct k__tmpl_array *arr, size_t idx, const k__tmpl_array_elem *elem) {
    return k__tmpl_array_(insert_all)(arr, idx, elem, 1);
}

#endif

k__tmpl_array_fn int k__tmpl_array_(push_back_all)(struct k__tmpl_array *arr, k__tmpl_array_elem *elems, size_t elems_num) {
    return k__tmpl_array_(insert_all)(arr, arr->size, elems, elems_num);
}

#if ! defined(K_TMPL_ARRAY_PASS_ELEM_BY_PTR)

k__tmpl_array_fn int k__tmpl_array_(push_back)(struct k__tmpl_array *arr, k__tmpl_array_elem elem) {
    return k__tmpl_array_(insert_all)(arr, arr->size, &elem, 1);
}

#else

k__tmpl_array_fn int k__tmpl_array_(push_back)(struct k__tmpl_array *arr, const k__tmpl_array_elem *elem) {
    return k__tmpl_array_(insert_all)(arr, arr->size, elem, 1);
}

#endif

k__tmpl_array_fn void k__tmpl_array_(shift_left)(struct k__tmpl_array *arr, size_t idx, size_t n) {
    assert(NULL != arr);
    assert(idx <= arr->size);
    assert(n <= idx);

    void  *shift_from = &arr->storage[idx];
    void  *shift_to   = &arr->storage[idx - n];
    size_t shift_size = (arr->size - idx) * sizeof(k__tmpl_array_elem);
    memmove(shift_to, shift_from, shift_size);

    arr->size -= n;
}

k__tmpl_array_fn void k__tmpl_array_(remove_range)(struct k__tmpl_array *arr, size_t idx_from, size_t idx_to) {
    k__tmpl_array_(shift_left)(arr, idx_to, idx_to - idx_from);
}

k__tmpl_array_fn void k__tmpl_array_(remove)(struct k__tmpl_array *arr, size_t idx) {
    k__tmpl_array_(shift_left)(arr, idx + 1, 1);
}

k__tmpl_array_fn void k__tmpl_array_(pop_back)(struct k__tmpl_array *arr) {
    assert(NULL != arr);
    assert(0 != arr->size);

    arr->size -= 1;
}

k__tmpl_array_fn void k__tmpl_array_(clear)(struct k__tmpl_array *arr) {
    assert(NULL != arr);

    arr->size = 0;
}

k__tmpl_array_fn void k__tmpl_array_(free_storage)(struct k__tmpl_array *arr) {
    assert(NULL != arr);

    k__tmpl_array_fn_free(arr->storage);

    arr->capacity = 0;
    arr->size     = 0;
    arr->storage  = NULL;
}

#endif

/* endregion */

#undef k__tmpl_array
#undef k__tmpl_array_
#undef k__tmpl_array_elem
#undef k__tmpl_array_x_impl_
#undef k__tmpl_array_x_impl
#undef k__tmpl_array_fn_malloc
#undef k__tmpl_array_fn_free
#undef k__tmpl_array_fn

#undef K_TMPL_ARRAY_STRUCT_NAME
#undef K_TMPL_ARRAY_ELEM_TYPE
#undef K_TMPL_ARRAY_DEFINE_STRUCT
#undef K_TMPL_ARRAY_DEFINE_FUNCTION
#undef K_TMPL_ARRAY_FN_MALLOC
#undef K_TMPL_ARRAY_FN_FREE
#undef k_tmpl_array_pass_elem_by_val
#undef k_tmpl_array_static_fn

#endif
