#include <assert.h>
#include <limits.h>

#include "k_mem_pool.h"

/* 向系统申请分配的 chunk 的头部 */
struct k_mem_chunk {

    /* 链向下一个 chunk
     *
     * pool 所指向的第一个 chunk 是仍有剩余空间的 chunk，
     * 此后链上的 chunk 都是已用尽的 chunk。
     */
    struct k_mem_chunk *next;
};

/* 分配给用户使用的内存块的头部 */
struct k_mem_block {

    union {
        /* 分配出的 block 启用 `list` 字段
         *
         * 若 block 是从 chunk 中切出的，则 `list` 记录其所属的 free_list，归还时直接链入。
         * 若 block 由 `fn_malloc()` 分配，则 `list` 标记为 `NULL`。
         */
        struct k_mem_block **list;

        /* 已归还的 block 启用 `next` 字段
         *
         * 若 block 是从 chunk 中切出的，则归入 free_list，`next` 链向下一个 block。
         * 若 block 由 `fn_malloc()` 分配，则 `next` 字段不会被使用。
         */
        struct k_mem_block *next;
    };
};

/* 由 `fn_malloc()` 分配的 block 的头部 */
struct k_mem_heap_block {

    /* 记录该 block 所属的内存池 */
    struct k_mem_pool *pool;

    /* `next` 链向下一个 block，`pprev` 指向前一个 block 的 `next` 的地址 */
    struct k_mem_heap_block *next;
    struct k_mem_heap_block **pprev;

    /* 分配给用户使用的内存块的头部 */
    struct k_mem_block block;
};

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

#define align_up(n, x)   (((n) + (x) - 1) / (x) * (x))
#define align_down(n, x) ((n) / (x) * (x))

static int k__mem_pool_check_config(const struct k_mem_pool_config *config) {

    if (NULL == config->fn_malloc || NULL == config->fn_free)
        return -1;

    if (0 != config->alloc_size_align % 8)
        return -1; /* `alloc_size_align` 不是 8 的倍数 */

    if (0 != config->block_size_max % config->alloc_size_align)
        return -1; /* `block_size_max` 不是 `alloc_size_align` 的倍数 */

    if (0 != config->alloc_chunk_size % 8)
        return -1; /* `alloc_chunk_size` 不是 8 的倍数 */

    if (config->block_size_max <= config->alloc_size_align)
        return -1; /* `block_size_max` 值过小，比对齐倍数还小 */

    if (SIZE_MAX - sizeof(struct k_mem_block) <= config->block_size_max)
        return -1; /* `block_size_max` 值过大，整型计算发生上溢 */

    if (config->alloc_chunk_size < sizeof(struct k_mem_chunk))
        return -1; /* `alloc_chunk_size` 值过小，chunk 无法包含头部 */

    if (config->alloc_chunk_size - sizeof(struct k_mem_chunk) < config->block_size_max + sizeof(struct k_mem_block))
        return -1; /* `alloc_chunk_size` 值过小，chunk 无法切出最大的 block */

    return 0;
}

struct k_mem_pool *k_mem_pool_create(const struct k_mem_pool_config *config) {
    assert(NULL != config);

    if (0 != k__mem_pool_check_config(config))
        return NULL;

    size_t lists_num  = config->block_size_max / config->alloc_size_align + 1;
    size_t lists_size = lists_num * sizeof(struct k_mem_block *);

    /* 通过 create 方式创建的内存池，pool 结构体和 free_lists 共处同一个大内存块 */
    struct k_mem_pool *pool = config->fn_malloc(sizeof(struct k_mem_pool) + lists_size);
    if (NULL == pool)
        return NULL;

    struct k_mem_block **free_lists = ptr_offset(pool, sizeof(struct k_mem_pool));
    size_t chunk_capacity = config->alloc_chunk_size - sizeof(struct k_mem_chunk);

    pool->fn_malloc        = config->fn_malloc;
    pool->fn_free          = config->fn_free;
    pool->chunk_capacity   = chunk_capacity;
    pool->chunk_used       = chunk_capacity;
    pool->chunk            = NULL;
    pool->alloc_size_align = config->alloc_size_align;
    pool->block_size_max   = config->block_size_max;
    pool->free_lists       = free_lists;
    pool->heap_block_list  = NULL;

    size_t i = 0;
    for (; i < lists_num; i++) {
        free_lists[i] = NULL;
    }

    return pool;
}

struct k_mem_pool *k_mem_pool_construct(struct k_mem_pool *pool, const struct k_mem_pool_config *config) {
    assert(NULL != pool);
    assert(NULL != config);

    if (0 != k__mem_pool_check_config(config))
        return NULL;

    size_t lists_num  = config->block_size_max / config->alloc_size_align + 1;
    size_t lists_size = lists_num * sizeof(struct k_mem_block *);
    struct k_mem_block **free_lists = config->fn_malloc(lists_size);
    if (NULL == free_lists)
        return NULL;

    size_t chunk_capacity = config->alloc_chunk_size - sizeof(struct k_mem_chunk);

    pool->fn_malloc        = config->fn_malloc;
    pool->fn_free          = config->fn_free;
    pool->chunk_capacity   = chunk_capacity;
    pool->chunk_used       = chunk_capacity;
    pool->chunk            = NULL;
    pool->alloc_size_align = config->alloc_size_align;
    pool->block_size_max   = config->block_size_max;
    pool->free_lists       = free_lists;
    pool->heap_block_list  = NULL;

    size_t i = 0;
    for (; i < lists_num; i++) {
        free_lists[i] = NULL;
    }

    return pool;
}

static void k__mem_pool_free_all_chunks(struct k_mem_pool *pool) {
    struct k_mem_chunk *chunk = pool->chunk;
    while (NULL != chunk) {
        struct k_mem_chunk *next = chunk->next;
        pool->fn_free(chunk);
        chunk = next;
    }
}

static void k__mem_pool_free_all_heap_blocks(struct k_mem_pool *pool) {
    struct k_mem_heap_block *heap_block = pool->heap_block_list;
    while (NULL != heap_block) {
        struct k_mem_heap_block *next = heap_block->next;
        pool->fn_free(heap_block);
        heap_block = next;
    }
}

void k_mem_pool_destroy(struct k_mem_pool *pool) {

    if (NULL == pool)
        return;

    k__mem_pool_free_all_chunks(pool);
    k__mem_pool_free_all_heap_blocks(pool);
    pool->fn_free(pool);
}

void k_mem_pool_destruct(struct k_mem_pool *pool) {

    if (NULL == pool)
        return;

    k__mem_pool_free_all_chunks(pool);
    k__mem_pool_free_all_heap_blocks(pool);
    pool->fn_free(pool->free_lists);
}

static struct k_mem_block **k__mem_pool_select_free_list(struct k_mem_pool *pool, size_t block_size) {
    struct k_mem_block **free_lists = pool->free_lists;
    return &free_lists[(block_size + pool->alloc_size_align - 1) / pool->alloc_size_align];
}

static void *k__mem_pool_alloc_from_pool(struct k_mem_pool *pool, size_t size) {

    size_t block_size = align_up(size, pool->alloc_size_align);

    struct k_mem_block **list = k__mem_pool_select_free_list(pool, block_size);
    if (NULL != *list) {
        struct k_mem_block *block = *list;
        *list = block->next;

        block->list = list;
        return ptr_offset(block, sizeof(struct k_mem_block));
    }

    size_t remaining_size = pool->chunk_capacity - pool->chunk_used;
    size_t required_size  = sizeof(struct k_mem_block) + block_size;
    if (remaining_size < required_size) {

        struct k_mem_chunk *new_chunk = pool->fn_malloc(sizeof(struct k_mem_chunk) + pool->chunk_capacity);
        if (NULL == new_chunk)
            return NULL;

        if (sizeof(struct k_mem_block) < remaining_size) {
            size_t free_block_size = align_down(remaining_size - sizeof(struct k_mem_block), pool->alloc_size_align);
            if (pool->alloc_size_align <= free_block_size) {

                struct k_mem_block **free_list = k__mem_pool_select_free_list(pool, free_block_size);
                struct k_mem_block *free_block = ptr_offset(pool->chunk, sizeof(struct k_mem_chunk) + pool->chunk_used);

                free_block->next = *free_list;
                *free_list = free_block;
            }
        }

        new_chunk->next = pool->chunk;
        pool->chunk = new_chunk;
        pool->chunk_used = 0;
    }

    struct k_mem_block *block = ptr_offset(pool->chunk, sizeof(struct k_mem_chunk) + pool->chunk_used);
    pool->chunk_used += required_size;

    block->list = list;
    return ptr_offset(block, sizeof(struct k_mem_block));
}

static void *k__mem_pool_alloc_from_heap(struct k_mem_pool *pool, size_t size) {

    struct k_mem_heap_block *block = pool->fn_malloc(sizeof(struct k_mem_heap_block) + size);
        if (NULL == block)
            return NULL;

    block->block.list = NULL;
    block->pool = pool;

    if (pool->heap_block_list != NULL) {
        ((struct k_mem_heap_block *)(pool->heap_block_list))->pprev = &block->next;
    }
    block->next = pool->heap_block_list;
    pool->heap_block_list = block;
    block->pprev = (struct k_mem_heap_block **)(&pool->heap_block_list);

    return ptr_offset(block, sizeof(struct k_mem_heap_block));
}

void *k_mem_pool_alloc(struct k_mem_pool *pool, size_t size) {
    assert(NULL != pool);

    if (size <= pool->block_size_max) {
        return k__mem_pool_alloc_from_pool(pool, size);
    }
    else if (size < SIZE_MAX - sizeof(struct k_mem_heap_block)) {
        return k__mem_pool_alloc_from_heap(pool, size);
    }
    else {
        return NULL;
    }
}

void k_mem_pool_free(void *p) {

    if (NULL == p)
        return;

    struct k_mem_block *block = ptr_offset(p, -sizeof(struct k_mem_block));
    if (NULL != block->list) {
        struct k_mem_block **list = block->list;

        block->next = *list;
        *list = block;
    }
    else {
        struct k_mem_heap_block *heap_block = ptr_offset(p, -sizeof(struct k_mem_heap_block));

        if (NULL != heap_block->next) {
            heap_block->next->pprev = heap_block->pprev;
        }
        *(heap_block->pprev) = heap_block->next;

        heap_block->pool->fn_free(heap_block);
    }
}
