#ifndef K_ARRAY_H
#define K_ARRAY_H

#include <stddef.h>

/** \brief 支持动态扩容的数组容器 */
struct k_array {

    /* [private] */
    void *(*fn_malloc)(size_t size);

    /* [private] */
    void (*fn_free)(void *p);

    /** \brief [read-only] 数组所存储的元素的大小 */
    size_t elem_size;

    /** \brief [read-only] 数组容量，即在不重新分配存储的情况下最多能容纳的元素数量 */
    size_t capacity;

    /** \brief [read-only] 容器持有的元素的数量 */
    size_t size;

    /**
     * \brief [read-only] 指向存储元素的连续空间的指针
     *
     * 你可以对数组的 `storage` 指针做显式类型转换，配合下标运算符取得元素。
     * 若数组 `arr` 存储的元素类型为 `T`，可以这样操作取得索引为 `idx` 的元素：
     * ```C
     * T *elem_addr  = &((T *)arr->storage)[idx];
     * T  elem_value =  ((T *)arr->storage)[idx];
     * ```
     *
     * 或使用 `k_array_get_elem()` 和 `k_array_get_elem_addr()`。
     * ```C
     * T *elem_addr  = k_array_get_elem_addr(arr, idx);
     * T  elem_value = k_array_get_elem(arr, idx, T);
     * ```
     */
    void *storage;
};

/** \brief 用于构造数组的可选配置参数 */
struct k_array_options {

    /** \brief 内存分配函数 */
    void *(*fn_malloc)(size_t size);

    /** \brief 内存释放函数 */
    void (*fn_free)(void *p);
};

/**
 * \brief 创建数组
 *
 * `elem_size` 指定数组所存储的元素的大小，
 * `options` 为可选的配置参数，若为 `NULL` 则使用默认配置。
 *
 * 若创建成功，函数返回数组容器的指针，否则返回 `NULL`。
 */
struct k_array *k_array_create(size_t elem_size, const struct k_array_options *options);

/**
 * \brief 销毁数组
 *
 * 若 `arr` 为 `NULL`，则函数立即返回。
 */
void k_array_destroy(struct k_array *arr);

/**
 * \brief 构造数组
 *
 * 在 `arr` 所指向的内存段上原地构造数组。`elem_size` 指定数组所存储的元素的大小，
 * `options` 为可选的配置参数，若为 `NULL` 则使用默认配置。
 *
 * 若构造成功，函数返回值同入参 `arr`，否则返回 `NULL`。
 */
struct k_array *k_array_construct(struct k_array *arr, size_t elem_size, const struct k_array_options *options);

/**
 * \brief 析构数组
 *
 * 原地析构 `arr` 所指向的内存段上的动态数组。
 * 若 `arr` 为 `NULL`，则函数立即返回。
 */
void k_array_destruct(struct k_array *arr);

/**
 * \brief 获取数组中指定索引处的元素的地址
 *
 * `arr` 为指向数组的指针，`idx` 指定元素的索引。
 */
#define k_array_get_elem_addr(arr, idx) \
    (  (void *)((char *)(arr)->storage + ((idx) * (arr)->elem_size))  )

/**
 * \brief 获取数组中指定索引处的元素
 *
 * `arr` 为指向数组的指针，`idx` 指定元素的索引，`elem_type` 指明元素的类型。
 */
#define k_array_get_elem(arr, idx, elem_type) \
    (  ((elem_type *)((arr)->storage))[idx]  )

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
int k_array_reserve(struct k_array *arr, size_t n);

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
 * 若数组 `arr` 存储的元素类型为 `T`，要在索引 `idx` 处增加 `n` 个元素，可以这样操作：
 * ```C
 * T *insert_pos = k_array_shift_right(arr, idx, n);
 * if (NULL != insert_pos) {
 *     insert_pos[0]   = ...;
 *     insert_pos[1]   = ...;
 *     ...
 *     insert_pos[n-1] = ...;
 * }
 * ```
 */
void *k_array_shift_right(struct k_array *arr, size_t idx, size_t n);

/**
 * \brief 在数组指定位置插入多个元素
 *
 * 本函数将 `elems` 指向的内存段中的 `elems_num` 个元素复制插入到数组 `arr` 中索引为 `idx` 的位置，
 * 总共复制 `elems_num * arr->elem_size` 字节的内存数据。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_array_insert_all(struct k_array *arr, size_t idx, const void *elems, size_t elems_num);

/** \brief 在数组指定位置插入一个元素。若成功，函数返回 0，否则返回非 0 */
int k_array_insert(struct k_array *arr, size_t idx, const void *elem);

/** \brief 在数组尾部追加多个元素。若成功，函数返回 0，否则返回非 0 */
int k_array_push_back_all(struct k_array *arr, const void *elems, size_t elems_num);

/** \brief 在数组尾部追加一个元素。若成功，函数返回 0，否则返回非 0 */
int k_array_push_back(struct k_array *arr, const void *elem);

/**
 * \brief 将数组指定索引后的元素左移，覆盖删除前面的元素
 *
 * 将数组中索引 `idx` 及其往后的所有元素往数组头部方向移动 `n` 个位置，
 * 覆盖删除 `idx` 往左的 `n` 个元素。
 *
 * 调用本函数后，数组 `size` 减少 `n`。
 */
void k_array_shift_left(struct k_array *arr, size_t idx, size_t n);

/** \brief 删除数组中指定区间（左闭右开）内的元素 */
void k_array_remove_range(struct k_array *arr, size_t idx_from, size_t idx_to);

/** \brief 删除数组中指定索引处的元素 */
void k_array_remove(struct k_array *arr, size_t idx);

/** \brief 删除数组最末尾的元素 */
void k_array_pop_back(struct k_array *arr);

/** \brief 清空数组，但保持原容量，不释放存储元素所使用的内存 */
void k_array_clear(struct k_array *arr);

/** \brief 清空数组，并释放存储元素所用的内存 */
void k_array_free_storage(struct k_array *arr);

#endif
