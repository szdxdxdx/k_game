#ifndef K_MEM_POOL_H
#define K_MEM_POOL_H

#include <stddef.h>

/**
 * \brief 分级内存池
 *
 * 内存池会动态向系统申请分配大内存块 chunk，分配给用户的内存是从 chunk 中切出的小块 block。
 *
 * 分配内存时，若请求的 size 不超过 `block_size_max`，则从 chunk 中切出一块 block 分配给用户。
 * 分配出的 block 大小会向上对齐为 `alloc_size_align` 的倍数。
 *
 * 每个从 chunk 中切出的 block 都对应有一条 free_list。归还 block 时将其链入对应的 free_list。
 * free_list 使用单链串起一组空闲 block。分配内存时优先使用 free_list 中的空闲 block。
 *
 * 若申请的内存大小超过 `block_size_max`，内存池会调用 `fn_malloc()` 来分配 block。
 * 归还该 block 时，也不会将其链入 `free_list`，而是调用 `fn_free()`。
 *
 * 创建或构造内存池时不会预分配 chunk，等到用户第一次向内存池索要内存时才创建第一个 chunk。
 * 内存池使用期间不会归还 chunk 的内存，等到销毁或析构内存池时才归还所有 chunk。
 */
struct k_mem_pool {

    /** \brief [private] 内存分配函数 */
    void *(*fn_malloc)(size_t size);

    /** \brief [private] 内存释放函数 */
    void  (*fn_free)(void *p);

    /** \brief [read-only] 大内存块 chunk 的容量 */
    size_t chunk_capacity;

    /** \brief [read-only] 大内存块 chunk 已使用的大小 */
    size_t chunk_used;

    /** \brief [private] 当前可用的 chunk，该 chunk 用单链串起前一个的已用尽的 chunk */
    void *chunk;

    /** \brief [read-only] 对齐倍数 */
    size_t alloc_size_align;

    /** \brief [read-only] 从 chunk 中能切出最大的 block 的大小 */
    size_t block_size_max;

    /** \brief [private] free_list 的数组 */
    void *free_lists;

    /** \brief [private] 用双链串起所有由 `fn_malloc()` 分配出的 block */
    void *heap_block_list;
};

/** \brief 用于创建内存池的配置参数 */
struct k_mem_pool_config {

    /** \brief 内存分配函数 */
    void *(*fn_malloc)(size_t size);

    /** \brief 内存释放函数 */
    void (*fn_free)(void *p);

    /**
     * \brief 对齐倍数
     *
     * 内存池从 chunk 中分配出的 block 的大小会向上对齐到 `alloc_size_align` 的整数倍。例如：
     * - 若 `alloc_size_align` 为  8，用户申请 20 字节时，实际分配 24 字节。
     * - 若 `alloc_size_align` 为 16，用户申请 20 字节时，实际分配 32 字节。
     * - 若 `alloc_size_align` 为 16，用户申请 32 字节时，实际分配 32 字节。
     *
     * 要求 `alloc_size_align` 必须是 8 的倍数。
     */
    size_t alloc_size_align;

    /**
     * \brief 从 chunk 中能切出最大的 block 的大小
     *
     * 若请求的内存不超过 `block_size_max`，则分配出的 block 来自 chunk，
     * 否则内存池会求助 `fn_malloc()`。例如：
     * - 若 `block_size_max` 为 128，用户申请 130 字节时，分配出的 block 来自 `fn_malloc()`。
     * - 若 `block_size_max` 为 128，用户申请 128 字节时，分配出的 block 来自 chunk。
     * - 若 `block_size_max` 为 128，用户申请 120 字节时，分配出的 block 来自 chunk。
     *
     * 要求 `block_size_max` 必须大于 `alloc_size_align`，且是 `alloc_size_align` 的倍数。
     */
    size_t block_size_max;

    /**
     * \brief 创建新 chunk 时，向 `fn_malloc()` 申请的大小
     *
     * - 若 `alloc_chunk_size` 为 1024，则创建 chunk 时，向 `fn_malloc()` 申请 1024 字节。
     * - 若 `alloc_chunk_size` 为  512，则创建 chunk 时，向 `fn_malloc()` 申请  512 字节。
     *
     * 实际上，当 `alloc_chunk_size` 为 1024 且 `block_size_max` 为 128 时，
     * 一个 chunk 并不能恰好分配出 8 个 128 字节的 block。
     * 因为每个 block 包含一个头部，chunk 裁切的大小实际上略大于申请的大小。
     * 而每个 chunk 也包含一个头部，用于切割的区域是去除头部后的部分，略小于 `alloc_chunk_size`；
     *
     * 要求 `alloc_chunk_size` 必须是 8 的倍数，
     * `alloc_chunk_size` 必须大于 `block_size_max`，建议大很多倍。
     */
    size_t alloc_chunk_size;
};

/**
 * \brief 创建内存池
 *
 * 若成功，函数返回内存池指针，否则返回 `NULL`。
 */
struct k_mem_pool *k_mem_pool_create(const struct k_mem_pool_config *config);

/**
 * \brief 销毁内存池
 *
 * 销毁内存池。所有通过该内存池分配出的内存块将被一并回收，无需逐一归还。
 *
 * 若 `pool` 为 `NULL`，则函数立即返回。
 */
void k_mem_pool_destroy(struct k_mem_pool *pool);

/**
 * \brief 构造内存池
 *
 * 在 `pool` 所指向的内存段上原地构造内存池。
 *
 * 若成功，函数返回值同入参 `pool`，否则返回 `NULL`。
 */
struct k_mem_pool *k_mem_pool_construct(struct k_mem_pool *pool, const struct k_mem_pool_config *config);

/**
 * \brief 析构内存池
 *
 * 原地析构 `pool` 所指向的内存段上的内存池，
 * 所有通过该内存池分配出的内存块将被一并回收，无需逐一归还。
 *
 * 若 `pool` 为 `NULL`，则函数立即返回。
 */
void k_mem_pool_destruct(struct k_mem_pool *pool);

/**
 * \brief 申请内存块
 *
 * 向内存池申请分配 `size` 字节的内存块。
 *
 * 若申请的大小超过 `block_size_max`，则内存池求助 `fn_malloc()`。
 * 否则分配从 chunk 中切出的小块 block。
 *
 * 若分配成功，函数返回内存块指针，否则返回 `NULL`。
 *
 * 特殊地，若申请 0 字节大小的内存块，函数可能返回非 `NULL`,
 * 但该指针指向的内存块无法存储任何数据，你不应读写该内存块中的内容。
 */
void *k_mem_pool_alloc(struct k_mem_pool *pool, size_t size);

/**
 * \brief 归还内存块
 *
 * 向内存池归还指针 `p` 所指向的内存块。
 * 请勿重复归还！归还后不应再读写该内存块中的内容！
 *
 * 若 `p` 为 `NULL`，则函数立即返回。
 */
void k_mem_pool_free(void *p);

#endif
